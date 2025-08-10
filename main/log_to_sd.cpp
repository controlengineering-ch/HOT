// log_to_sd.cpp (ESP-IDF 5.x, ESP32-C6 + SDSPI)
#include <cstdio>
#include <cstring>
#include <string>

extern "C" {
#include "esp_log.h"
#include "esp_err.h"
#include "esp_vfs_fat.h"
#include "driver/spi_common.h"
#include "driver/sdspi_host.h"
#include "sdmmc_cmd.h"
#include "esp_timer.h"
}

static const char* TAG = "storage";
static std::string s_mount_point;

// --- PINNIT: säädä omien kytkentöjesi mukaan ---
#ifndef SD_PIN_MOSI
#define SD_PIN_MOSI  6   // esimerkki
#endif
#ifndef SD_PIN_MISO
#define SD_PIN_MISO  5   // esimerkki
#endif
#ifndef SD_PIN_SCLK
#define SD_PIN_SCLK  4   // esimerkki
#endif
#ifndef SD_PIN_CS
#define SD_PIN_CS    3   // esimerkki (CS)
#endif
// ------------------------------------------------

bool initSDCard(const char* mount_point)
{
    s_mount_point = mount_point ? mount_point : "/sdcard";

    esp_vfs_fat_mount_config_t mount_config = {
        .format_if_mount_failed   = true,
        .max_files                = 5,
        .allocation_unit_size     = 16 * 1024,
        .disk_status_check_enable = false,
        .use_one_fat              = false
    };

    sdmmc_card_t* card = nullptr;

    // SDSPI host
    sdmmc_host_t host = SDSPI_HOST_DEFAULT();
    // ESP32-C6: käytä SPI2_HOST tai SPI3_HOST laitteistosi mukaan
    const spi_host_device_t spi_host = SPI2_HOST;
    host.slot = spi_host; // sdmmc_host_t.slot on int, mutta arvo on sama

    // SPI-väylä (zero-init -> ei varoituksia)
    spi_bus_config_t bus_cfg = {};
    bus_cfg.mosi_io_num     = SD_PIN_MOSI;
    bus_cfg.miso_io_num     = SD_PIN_MISO;
    bus_cfg.sclk_io_num     = SD_PIN_SCLK;
    bus_cfg.quadwp_io_num   = -1;
    bus_cfg.quadhd_io_num   = -1;
    bus_cfg.max_transfer_sz = 4000;

    // HUOM: spi_bus_initialize odottaa spi_host_device_t
    esp_err_t err = spi_bus_initialize(spi_host, &bus_cfg, SPI_DMA_CH_AUTO);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "spi_bus_initialize failed: %s", esp_err_to_name(err));
        return false;
    }

    // SD-laitteen konfiguraatio
    sdspi_device_config_t slot_config = SDSPI_DEVICE_CONFIG_DEFAULT();
    slot_config.gpio_cs = static_cast<gpio_num_t>(SD_PIN_CS);  // tyyppicasti
    slot_config.host_id = spi_host;                            // oikea tyyppi

    // Mounttaa FAT VFS
    err = esp_vfs_fat_sdspi_mount(s_mount_point.c_str(), &host, &slot_config,
                                  &mount_config, &card);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "esp_vfs_fat_sdspi_mount failed: %s", esp_err_to_name(err));
        spi_bus_free(spi_host);
        return false;
    }

    sdmmc_card_print_info(stdout, card);
    ESP_LOGI(TAG, "SD mounted at %s", s_mount_point.c_str());
    return true;
}

bool logSensorData(const std::string& identifier,
                   float radon,
                   float temperature,
                   float humidity,
                   float pressure)
{
    if (s_mount_point.empty()) {
        ESP_LOGE(TAG, "SD not mounted. Call initSDCard() first.");
        return false;
    }

    const uint64_t ms = esp_timer_get_time() / 1000ULL;

    std::string path = s_mount_point + "/sensors.csv";
    FILE* f = std::fopen(path.c_str(), "a");
    if (!f) {
        ESP_LOGE(TAG, "fopen('%s') failed", path.c_str());
        return false;
    }

    std::fseek(f, 0, SEEK_END);
    if (std::ftell(f) == 0) {
        std::fprintf(f, "timestamp_ms,identifier,radon,temperature,humidity,pressure\n");
    }

    int n = std::fprintf(f, "%llu,%s,%.2f,%.2f,%.2f,%.2f\n",
                         static_cast<unsigned long long>(ms),
                         identifier.c_str(),
                         static_cast<double>(radon),
                         static_cast<double>(temperature),
                         static_cast<double>(humidity),
                         static_cast<double>(pressure));
    std::fclose(f);

    if (n <= 0) {
        ESP_LOGE(TAG, "fprintf failed");
        return false;
    }

    ESP_LOGI(TAG, "Logged to %s", path.c_str());
    return true;
}

