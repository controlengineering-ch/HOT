
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "vibra.h"

void app_main(void)
{
    vibra_sensor_init();
    while (1) {
        if (vibra_sensor_triggered()) {
            printf("Tärinä havaittu!\n");
        }
        vTaskDelay(pdMS_TO_TICKS(200));
    }
}
