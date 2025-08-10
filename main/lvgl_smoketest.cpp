#include "lvgl.h"        // LVGL v9: käytä "lvgl.h"
#include "panel_init.h"  // sinun komponentti
#include "esp_log.h"
#include "esp_timer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static const char* TAG = "LVGL_SMOKETEST";

extern "C" void app_main(void)
{
    ESP_LOGI(TAG, "Starting LVGL smoketest...");

    // Alusta paneeli + LVGL (panel_init() hoitaa lv_init() ja tick-timerin)
    panel_init_default();
    panel_backlight_set(100);

    // Teksti
    lv_obj_t* label = lv_label_create(lv_screen_active());
    lv_label_set_text(label, "Hello LVGL!");
    lv_obj_align(label, LV_ALIGN_CENTER, 0, -20);

    // Palkki
    lv_obj_t* bar = lv_bar_create(lv_screen_active());
    lv_obj_set_size(bar, 140, 14);
    lv_obj_align(bar, LV_ALIGN_CENTER, 0, 20);
    lv_bar_set_range(bar, 0, 100);
    lv_bar_set_value(bar, 0, LV_ANIM_OFF);

    int val = 0, step = 2;
    uint32_t prev_ms = 0;

    while (true) {
        // LVGL:n handleri
        lv_timer_handler();
        vTaskDelay(pdMS_TO_TICKS(5));

        // ~20 ms välein animaatio
        uint32_t now_ms = (uint32_t)(esp_timer_get_time() / 1000);
        if (now_ms - prev_ms >= 20) {
            prev_ms = now_ms;
            val += step;
            if (val >= 100 || val <= 0) step = -step;
            lv_bar_set_value(bar, val, LV_ANIM_OFF);
        }
    }
}

