#include "ble_scanner.h"
#include "esp_log.h"
#include "esp_bt.h"
#include "nvs_flash.h"
#include "esp_gap_ble_api.h"

static const char* TAG = "BLE_SCAN";
static std::vector<SensorInfo> foundSensors;

static void gapCallback(esp_gap_ble_cb_event_t event, esp_ble_gap_cb_param_t* param) {
    if (event == ESP_GAP_BLE_SCAN_RESULT_EVT) {
        auto result = param->scan_rst;
        if (result.search_evt == ESP_BT_GAP_DISC_RES_EVT || result.search_evt == ESP_GAP_SEARCH_INQ_RES_EVT) {
            char addr_str[18];
            snprintf(addr_str, sizeof(addr_str), "%02X:%02X:%02X:%02X:%02X:%02X",
                     result.bda[0], result.bda[1], result.bda[2],
                     result.bda[3], result.bda[4], result.bda[5]);

            int rssi = result.rssi;

            // Airthings Wave Plus tunnistus voidaan tarkentaa myöhemmin valmistajan mainoksilla

            foundSensors.push_back({addr_str, rssi});
            ESP_LOGI(TAG, "Found: %s RSSI: %d", addr_str, rssi);
        }
    }
}

void startBleScan() {
    ESP_ERROR_CHECK(nvs_flash_init());
    ESP_ERROR_CHECK(esp_bt_controller_mem_release(ESP_BT_MODE_CLASSIC_BT));
    esp_bt_controller_config_t bt_cfg = BT_CONTROLLER_INIT_CONFIG_DEFAULT();

    if (esp_bt_controller_init(&bt_cfg) != ESP_OK) {
        ESP_LOGE(TAG, "Bluetooth controller init failed");
        return;
    }

    esp_bt_controller_enable(ESP_BT_MODE_BLE);
    esp_bluedroid_init();
    esp_bluedroid_enable();

    esp_ble_gap_register_callback(gapCallback);
    esp_ble_gap_set_scan_params(&ble_scan_params_t{
        .scan_type = BLE_SCAN_TYPE_ACTIVE,
        .own_addr_type = BLE_ADDR_TYPE_PUBLIC,
        .scan_filter_policy = BLE_SCAN_FILTER_ALLOW_ALL,
        .scan_interval = 0x50,
        .scan_window = 0x30,
        .scan_duplicate = BLE_SCAN_DUPLICATE_DISABLE
    });

    ESP_LOGI(TAG, "BLE Scan started...");
}

std::vector<SensorInfo> getDiscoveredSensors() {
    return foundSensors;
}

