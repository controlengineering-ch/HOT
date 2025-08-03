#pragma once
#include "ble_scan.h"

bool init_sd_card();
void save_data_to_sd(const SensorData &data);
void load_config_from_sd();

