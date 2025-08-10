// main/ble_scan.cpp
#include <cstring>
#include <string>
#include <atomic>
#include "ble_sensor.h" 

extern "C" {
#include "esp_log.h"
#include "nvs_flash.h"

#include "nimble/ble.h"
#include "host/ble_hs.h"
#include "host/ble_gap.h"
#include "host/ble_gatt.h"
#include "services/gap/ble_svc_gap.h"
#include "services/gatt/ble_svc_gatt.h"
#include "nimble/nimble_port.h"
#include "nimble/nimble_port_freertos.h"
#include "esp_nimble_hci.h"
}

static const char *TAG = "NUS";

static uint8_t s_own_addr_type;
static std::atomic<uint16_t> s_conn_handle{BLE_HS_CONN_HANDLE_NONE};

static uint16_t s_svc_start = 0, s_svc_end = 0;
static uint16_t s_rx_val_h = 0;   // write
static uint16_t s_tx_val_h = 0;   // notify
static uint16_t s_tx_ccc_h = 0;   // 0x2902 descriptor

// NUS UUID:t little-endianinä (NimBLE:n makrolle)
static const ble_uuid128_t NUS_SVC_UUID =
    BLE_UUID128_INIT(0x9e,0xca,0xdc,0x24,0x0e,0xe5,0xa9,0xe0,0x93,0xf3,0xa3,0xb5,0x01,0x00,0x40,0x6e);
static const ble_uuid128_t NUS_RX_UUID =
    BLE_UUID128_INIT(0x9e,0xca,0xdc,0x24,0x0e,0xe5,0xa9,0xe0,0x93,0xf3,0xa3,0xb5,0x02,0x00,0x40,0x6e);
static const ble_uuid128_t NUS_TX_UUID =
    BLE_UUID128_INIT(0x9e,0xca,0xdc,0x24,0x0e,0xe5,0xa9,0xe0,0x93,0xf3,0xa3,0xb5,0x03,0x00,0x40,0x6e);

static void start_scan();

// -----------------------------------------------------------------------------
// Saapuvat notifikaatiot -> tee omasi tähän
// -----------------------------------------------------------------------------
__attribute__((weak)) void ble_nus_on_rx(const uint8_t* data, size_t len) {
    // Oletus: tulostetaan lokiin
    std::string s((const char*)data, (const char*)data + len);
    ESP_LOGI(TAG, "RX notify (%u B): %s", (unsigned)len, s.c_str());
}

// -----------------------------------------------------------------------------
// GATT client -tapahtumat (discovery, subscribe, notify…)
// -----------------------------------------------------------------------------
static int gattc_event(struct ble_gattc_event *event, void *arg) {
    switch (event->type) {
    case BLE_GATTC_EVENT_DISC_SVC:
        if (ble_uuid_cmp(event->disc_svc.svc->uuid, &NUS_SVC_UUID.u) == 0) {
            s_svc_start = event->disc_svc.svc->start_handle;
            s_svc_end   = event->disc_svc.svc->end_handle;
            ESP_LOGI(TAG, "NUS service: start=0x%04x end=0x%04x", s_svc_start, s_svc_end);
        }
        return 0;

    case BLE_GATTC_EVENT_DISC_SVC_CMPL:
        if (s_svc_start && s_svc_end) {
            // Discover kaikki karakteristiikat palvelusta
            return ble_gattc_disc_all_chrs(s_conn_handle.load(), s_svc_start, s_svc_end, gattc_event, nullptr);
        } else {
            ESP_LOGW(TAG, "NUS service ei löytynyt – katkaistaan");
            ble_gap_terminate(s_conn_handle.load(), BLE_ERR_REM_USER_CONN_TERM);
        }
        return 0;

    case BLE_GATTC_EVENT_DISC_CHR: {
        const auto *chr = event->disc_chr.chr;
        if (ble_uuid_cmp(chr->uuid, &NUS_RX_UUID.u) == 0) {
            s_rx_val_h = chr->val_handle;
            ESP_LOGI(TAG, "NUS RX (write) val_handle=0x%04x props=0x%02x", s_rx_val_h, chr->properties);
        } else if (ble_uuid_cmp(chr->uuid, &NUS_TX_UUID.u) == 0) {
            s_tx_val_h = chr->val_handle;
            ESP_LOGI(TAG, "NUS TX (notify) val_handle=0x%04x props=0x%02x", s_tx_val_h, chr->properties);
        }
        return 0;
    }

    case BLE_GATTC_EVENT_DISC_CHR_CMPL:
        if (s_tx_val_h) {
            // Etsi CCCD (0x2902) tx-karun descriptor-alueelta: (val+1..svc_end)
            return ble_gattc_disc_all_dscs(s_conn_handle.load(),
                                           s_tx_val_h + 1, s_svc_end, gattc_event, nullptr);
        } else {
            ESP_LOGW(TAG, "TX-karua ei löytynyt");
        }
        return 0;

    case BLE_GATTC_EVENT_DISC_DSC: {
        const auto *dsc = event->disc_dsc.dsc;
        if (ble_uuid_u16(dsc->uuid) == BLE_GATT_DSC_CLT_CFG_UUID16) {
            s_tx_ccc_h = dsc->handle;
            ESP_LOGI(TAG, "TX CCCD handle=0x%04x", s_tx_ccc_h);
        }
        return 0;
    }

    case BLE_GATTC_EVENT_DISC_DSC_CMPL:
        if (s_tx_ccc_h) {
            // Ota notifyt päälle
            uint8_t ccc[2] = {0x01, 0x00}; // notifications on
            int rc = ble_gattc_write_flat(s_conn_handle.load(), s_tx_ccc_h, ccc, sizeof ccc);
            if (rc == 0) ESP_LOGI(TAG, "Notifications ENABLED");
            else ESP_LOGE(TAG, "CCCD write failed rc=%d", rc);
        }
        return 0;

    case BLE_GATTC_EVENT_NOTIFY:
        ble_nus_on_rx(event->notify.value, event->notify.value_len);
        return 0;

    default:
        return 0;
    }
}

// -----------------------------------------------------------------------------
// GAP-tapahtumat (scan, connect, disconnect…)
// -----------------------------------------------------------------------------
static int gap_event(struct ble_gap_event *event, void *arg) {
    switch (event->type) {
    case BLE_GAP_EVENT_DISC: {
        // Parsitaan adv-kentät ja etsitään 128-bittisistä NUS-UUID
        struct ble_hs_adv_fields f;
        if (ble_hs_adv_parse_fields(&f, event->disc.data, event->disc.length_data) == 0) {
            for (int i = 0; i < f.num_uuids128; ++i) {
                if (ble_uuid_cmp(&f.uuids128[i].u, &NUS_SVC_UUID.u) == 0) {
                    char addr_str[BLE_ADDR_STR_LEN];
                    ble_addr_to_str(&event->disc.addr, addr_str);
                    ESP_LOGI(TAG, "NUS mainos löytyi: %s RSSI=%d", addr_str, event->disc.rssi);

                    // Yhdistä
                    ble_gap_disc_cancel();
                    struct ble_gap_conn_params cp = {
                        .scan_itvl = 0x0030,
                        .scan_window = 0x0030,
                        .itvl_min = 24, .itvl_max = 40,
                        .latency = 0,
                        .supervision_timeout = 400,
                        .min_ce_len = 0, .max_ce_len = 0,
                    };
                    int rc = ble_gap_connect(s_own_addr_type, &event->disc.addr, 30000, &cp, gap_event, nullptr);
                    if (rc) {
                        ESP_LOGE(TAG, "ble_gap_connect rc=%d", rc);
                        start_scan();
                    }
                    return 0;
                }
            }
        }
        return 0;
    }

    case BLE_GAP_EVENT_CONNECT:
        if (event->connect.status == 0) {
            s_conn_handle = event->connect.conn_handle;
            char addr_str[BLE_ADDR_STR_LEN];
            ble_addr_to_str(&event->connect.peer_addr, addr_str);
            ESP_LOGI(TAG, "Yhdistetty: handle=%u, peer=%s", s_conn_handle.load(), addr_str);

            // Palvelun discovery
            s_svc_start = s_svc_end = s_rx_val_h = s_tx_val_h = s_tx_ccc_h = 0;
            ble_gattc_disc_svc_by_uuid(s_conn_handle.load(), &NUS_SVC_UUID.u, gattc_event, nullptr);
        } else {
            ESP_LOGW(TAG, "Connect epäonnistui, status=%d", event->connect.status);
            start_scan();
        }
        return 0;

    case BLE_GAP_EVENT_DISCONNECT:
        ESP_LOGW(TAG, "Disconnect: reason=%d", event->disconnect.reason);
        s_conn_handle = BLE_HS_CONN_HANDLE_NONE;
        start_scan();
        return 0;

    case BLE_GAP_EVENT_DISC_COMPLETE:
        // jatka skannausta automaattisesti
        start_scan();
        return 0;

    default:
        return 0;
    }
}

// -----------------------------------------------------------------------------
// Skannauksen käynnistys
// -----------------------------------------------------------------------------
static void start_scan() {
    struct ble_gap_disc_params p = {
        .itvl = 0x0030,           // ~30 ms
        .window = 0x0030,         // aktiivinen skannaus
        .filter_policy = 0,
        .limited = 0,
        .passive = 0,
        .filter_duplicates = 1,
    };
    int rc = ble_gap_disc(s_own_addr_type, BLE_HS_FOREVER, &p, gap_event, nullptr);
    if (rc) ESP_LOGE(TAG, "ble_gap_disc rc=%d", rc);
    else    ESP_LOGI(TAG, "Scan ON (NUS-filter adv-parse)");
}

// -----------------------------------------------------------------------------
// Host-task
// -----------------------------------------------------------------------------
static void host_task(void *param) {
    nimble_port_run();           // ei palaa
    nimble_port_freertos_deinit();
}

// -----------------------------------------------------------------------------
// Julkinen API
// -----------------------------------------------------------------------------
bool ble_nus_is_connected() {
    return s_conn_handle.load() != BLE_HS_CONN_HANDLE_NONE && s_rx_val_h && s_tx_val_h;
}

bool ble_nus_send(const uint8_t *data, size_t len, bool no_rsp = false) {
    if (!ble_nus_is_connected()) return false;
    int rc = no_rsp
        ? ble_gattc_write_no_rsp_flat(s_conn_handle.load(), s_rx_val_h, data, len)
        : ble_gattc_write_flat(s_conn_handle.load(), s_rx_val_h, data, len);
    if (rc) ESP_LOGE(TAG, "TX write rc=%d", rc);
    return rc == 0;
}

bool ble_nus_send_str(const char *s) {
    return ble_nus_send(reinterpret_cast<const uint8_t*>(s), strlen(s), true);
}

void ble_nus_disconnect() {
    if (s_conn_handle.load() != BLE_HS_CONN_HANDLE_NONE) {
        ble_gap_terminate(s_conn_handle.load(), BLE_ERR_REM_USER_CONN_TERM);
    }
}

bool ble_nus_init_and_scan(const char *dev_name = "RadonSafe") {
    // NVS BLE:n taustamuistille
    if (nvs_flash_init() != ESP_OK) {
        nvs_flash_erase();
        nvs_flash_init();
    }

    ESP_ERROR_CHECK(esp_nimble_hci_and_controller_init());
    ESP_ERROR_CHECK(nimble_port_init());

    ble_hs_cfg.reset_cb = [](int reason) {
        ESP_LOGW(TAG, "reset_cb reason=%d", reason);
    };
    ble_hs_cfg.sync_cb = []() {
        int rc = ble_hs_id_infer_auto(0, &s_own_addr_type);
        assert(rc == 0);
        uint8_t addr_val[6];
        ble_hs_id_copy_addr(s_own_addr_type, addr_val, nullptr);
        ESP_LOGI(TAG, "Oma addr type=%u, %02X:%02X:%02X:%02X:%02X:%02X",
                 s_own_addr_type, addr_val[5],addr_val[4],addr_val[3],addr_val[2],addr_val[1],addr_val[0]);

        // Peruspalvelut ja nimi
        ble_svc_gap_init();
        ble_svc_gatt_init();
        ble_svc_gap_device_name_set(dev_name);

        start_scan();
    };

    nimble_port_freertos_init(host_task);
    return true;
}

