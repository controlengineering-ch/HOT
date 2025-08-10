#pragma once
#include <string>

// Mount SD card (stub: returns true even if already mounted elsewhere)
// Pass mount point like "/sdcard"
bool initSDCard(const char* mount_point);

// Append one CSV line with current timestamp (epoch ms) and sensor values
// csv_path example: "/sdcard/data/radon_log.csv"
bool logSensorData(const std::string& csv_path,
                   float radon_bq_m3,
                   float co2_ppm,
                   float temp_c,
                   float humidity_pct);

