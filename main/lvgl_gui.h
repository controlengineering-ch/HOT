#pragma once

#include "lvgl.h"

enum class StatusLevel {
    NORMAL,
    WARNING,
    CRITICAL
};

void lvgl_gui_init();
void update_radiation_value(int bq_value);
void update_sensor_id(const char* id);
void update_status(StatusLevel level);

