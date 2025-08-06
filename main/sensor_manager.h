#pragma once

typedef enum {
    SENSOR_NO7 = 0,
    SENSOR_NO8,
    SENSOR_NO1,
    SENSOR_MAX
} sensor_id_t;

void sensor_manager_start(sensor_id_t id);
void sensor_manager_stop(sensor_id_t id);
void sensor_manager_stop_all(void);
bool sensor_manager_is_running(sensor_id_t id);
void sensor_manager_print_status(void);
void sensor_manager_init(void);
