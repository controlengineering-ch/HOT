#include "log_to_sd.h"
#include <stdio.h>
#include <string>
#include <ctime>
#include "esp_vfs_fat.h"
#include "sdmmc_cmd.h"
#include "driver/sdmmc_host.h"
#include "driver/sdmmc_defs.h"
#include "esp_log.h"

#define MOUNT_POINT "/sdcard"
static const char* TAG = "SD_LOG";

bool initSDCard(const char* mountPath) {
    esp_vfs_fat_sdmmc_mount_config_t mount_config = {
        .format_if_mount_failed = false,
        .max_files = 5,
        .allocation_unit_size = 16 * 1024
    };

    sdmmc_card_t* card;
    const char mount_point[] = MOUNT_POINT;

    ESP_LOGI(TAG, "Mounting SD card at %s...", mount_point);

    esp_err_t ret = esp_vfs_fat_sdmmc_mount(mount_point, &sdmmc_host_default(), NULL, &mount_config, &card);

    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to mount filesystem. Error: %s", esp_err_to_name(ret));
        return false;
    }

    sdmmc_card_print_info(stdout, card);
    return true;
}

bool logSensorData(const std::string& sensorID, float radon, float co2, float temp, float humidity) {
    char filepath[128];
    snprintf(filepath, sizeof(filepath), "%s/RadonSafe/logs/sensor_log.csv", MOUNT_POINT);

    FILE* f = fopen(filepath, "a");
    if (!f) {
        ESP_LOGE(TAG, "Failed to open file for writing: %s", filepath);
        return false;
    }

    time_t now = time(NULL);
    struct tm* timeinfo = localtime(&now);

    char timeStr[64];
    strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", timeinfo);

    fprintf(f, "%s,%s,%.1f,%.1f,%.1f,%.1f\n", timeStr, sensorID.c_str(), radon, co2, temp, humidity);
    fclose(f);
    return true;
}

