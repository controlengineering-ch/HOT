#pragma once
#include "ble_scan.h"

void init_wifi();
bool is_wifi_connected();
void upload_data_to_cloud(const SensorData &data);

