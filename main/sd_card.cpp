#include <stdio.h>
#include <string>
#include "esp_vfs_fat.h"
#include "driver/sdspi_host.h"
#include "driver/spi_common.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "sdmmc_cmd.h"
#include "log_to_sd.h"
#include "data_structs.h"

static const char *TAG = "SD_CARD";

#define MOUNT_POINT "/sdcard"
#define SPI_DMA_CHAN 1

sdmmc_card_t *card;

bool init_sd_card() {
    esp_vfs_fat_mount_config_t mount_config = {
        .format_if_mount_failed = true,
        .max_files = 5,
        .disk_status_check_enable = false,
        .use_one_fat = false
    };

    spi_bus_config_t bus_cfg = {
        .mosi_io_num = GPIO_NUM_13,
        .miso_io_num = GPIO_NUM_15,
        .sclk_io_num = GPIO_NUM_14,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = 4000
    };

    ESP_ERROR_CHECK(spi_bus_initialize(SPI2_HOST, &bus_cfg, SPI_DMA_CHAN));

    sdspi_device_config_t slot_config = SDSPI_DEVICE_CONFIG_DEFAULT();
    slot_config.host_id = SPI2_HOST;
    slot_config.gpio_cs = GPIO_NUM_12;

    esp_err_t ret = esp_vfs_fat_sdspi_mount(MOUNT_POINT, &slot_config.host, &slot_config, &mount_config, &card);

    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to mount filesystem. Error: %s", esp_err_to_name(ret));
        return false;
    }

    ESP_LOGI(TAG, "SD card mounted at %s", MOUNT_POINT);
    return true;
}

void save_data_to_sd(const SensorData& data) {
    char path[128];
    snprintf(path, sizeof(path), "%s/data.csv", MOUNT_POINT);
    FILE* f = fopen(path, "a");
    if (f == NULL) {
        ESP_LOGE(TAG, "Failed to open file for writing: %s", path);
        return;
    }
    fprintf(f, "%s,%d,%llu\n", data.identifier.c_str(), data.radon, (unsigned long long)data.timestamp);
    fclose(f);
    ESP_LOGI(TAG, "Data written to %s", path);
}

void load_config_from_sd() {
    char path[128];
    snprintf(path, sizeof(path), "%s/config.txt", MOUNT_POINT);
    FILE* f = fopen(path, "r");
    if (f == NULL) {
        ESP_LOGW(TAG, "No config file found: %s", path);
        return;
    }

    int polling = 10, warn = 150, danger = 220;
    fscanf(f, "%d,%d,%d", &polling, &warn, &danger);
    fclose(f);

    set_control_params(polling, warn, danger);
    ESP_LOGI(TAG, "Config loaded: poll=%d, warn=%d, danger=%d", polling, warn, danger);
}

