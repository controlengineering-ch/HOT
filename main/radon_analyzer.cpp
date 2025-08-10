#include "radon_analyzer.h"
#include "radon_alert.h"
#include "driver/gpio.h"
#include "esp_log.h"

#ifndef RADONSAFE_ALERT_LED_GPIO
#define RADONSAFE_ALERT_LED_GPIO GPIO_NUM_2   // Vaihda laitekohtaisesti
#endif

static const char* TAG = "radon_analyzer";

static void ensure_led_init() {
    static bool inited = false;
    if (!inited) {
        gpio_reset_pin(RADONSAFE_ALERT_LED_GPIO);
        gpio_set_direction(RADONSAFE_ALERT_LED_GPIO, GPIO_MODE_OUTPUT);
        inited = true;
        ESP_LOGI(TAG, "LED init on GPIO %d", (int)RADONSAFE_ALERT_LED_GPIO);
    }
}

void trigger_alert_visuals(const RadonAlert& alert) {
    ensure_led_init();

    switch (alert.level) {
        case AlertLevel::Critical:
            gpio_set_level(RADONSAFE_ALERT_LED_GPIO, 1);
            ESP_LOGW(TAG, "CRITICAL alert: %.1f Bq/m3 @ %ld", alert.value_bq_m3, (long)alert.sample_ts);
            break;
        case AlertLevel::Warning:
            gpio_set_level(RADONSAFE_ALERT_LED_GPIO, 1);  // halutessa vilkutus
            ESP_LOGW(TAG, "WARNING alert: %.1f Bq/m3 @ %ld", alert.value_bq_m3, (long)alert.sample_ts);
            break;
        case AlertLevel::Normal:
        default:
            gpio_set_level(RADONSAFE_ALERT_LED_GPIO, 0);
            ESP_LOGI(TAG, "NORMAL: %.1f Bq/m3 @ %ld", alert.value_bq_m3, (long)alert.sample_ts);
            break;
    }
}

