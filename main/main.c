#include <stdio.h>
#include "sensor_reader.h"
#include "lvgl_display.h"

void app_main(void) {
    // LVGL init
    lvgl_display_init();

    // Read radon value
    sensor_data_t sensor;
    if (read_radon_value("/sdcard/dreamREC/radon_latest.txt", &sensor)) {
        printf("Radon: %.1f Bq/m³ at %s\n", sensor.radon_value, sensor.timestamp);

        // Update display
        lvgl_display_update(sensor.radon_value);

        // Append to history
        append_to_history("/sdcard/dreamREC/history.csv", &sensor);
    } else {
        printf("Failed to read radon data\n");
    }
}

