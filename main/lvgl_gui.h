#pragma once
#include "radon_analyzer.h"
#include "ble_scan.h"

void init_lvgl();
void update_gui_with_data(const SensorData &data, const RadonAlert &alert);

