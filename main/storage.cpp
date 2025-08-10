#include "storage.hpp"
#include <cstdio>
#include <cstring>
#include <string>
extern "C" {
#include "esp_log.h"
#include "esp_timer.h"
}

static const char* TAG = "storage";

// Minimal stub: assume SD already mounted by platform code (or not required yet).
// Return true so the app can proceed; replace later with real esp_vfs_fat_sdmmc_mount if needed.
bool initSDCard(const char* mount_point) {
    if (!mount_point || std::strlen(mount_point) == 0) {
        ESP_LOGW(TAG, "initSDCard: empty mount point");
    } else {
        ESP_LOGI(TAG, "initSDCard: (stub) using mount point: %s", mount_point);
    }
    return true;
}

bool logSensorData(const std::string& csv_path,
                   float radon_bq_m3,
                   float co2_ppm,
                   float temp_c,
                   float humidity_pct) {
    if (csv_path.empty()) {
        ESP_LOGE(TAG, "logSensorData: empty path");
        return false;
    }

    // Ensure parent dirs exist in your deployment; here we just append.
    FILE* f = std::fopen(csv_path.c_str(), "a");
    if (!f) {
        ESP_LOGE(TAG, "logSensorData: fopen('%s') failed", csv_path.c_str());
        return false;
    }

    // Timestamp: epoch milliseconds (from esp_timer, not RTC/UTC)
    unsigned long long ms = static_cast<unsigned long long>(esp_timer_get_time() / 1000ULL);

    // CSV: epoch_ms,radon,co2,temp,humidity
    // Adjust format/precision as you like.
    int rc = std::fprintf(f, "%llu,%.1f,%.0f,%.1f,%.1f\n",
                          ms, radon_bq_m3, co2_ppm, temp_c, humidity_pct);
    std::fclose(f);

    if (rc <= 0) {
        ESP_LOGE(TAG, "logSensorData: fprintf failed");
        return false;
    }
    return true;
}

