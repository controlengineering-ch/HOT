#include "very_low_power_manager.h"
#include "esp_sleep.h"
#include "esp_log.h"
#include "esp_timer.h"
#include "driver/gpio.h"
#include "esp_wifi.h"
#include "esp_bt.h"
#include "lvgl_gui.h"

static const char* TAG = "VeryLowPowerManager";

// RTC-muistin avulla säilytetään aikaleimoja syväunien välillä
RTC_DATA_ATTR uint64_t last_ble_poll_us = 0;
RTC_DATA_ATTR uint64_t last_wifi_attempt_us = 0;

void init_power_management() {
    ESP_LOGI(TAG, "Initializing low power state...");
    lvgl_gui_display_off();
    gpio_set_level(GPIO_NUM_0, 0); // LED off
    esp_wifi_stop();
    esp_bt_controller_disable();
}

void enter_deep_sleep_with_timer(uint64_t sleep_us) {
    ESP_LOGI(TAG, "Entering deep sleep for %llu us", sleep_us);
    esp_deep_sleep(sleep_us);
}

void enable_gpio_wakeup(gpio_num_t pin) {
    gpio_wakeup_enable(pin, GPIO_INTR_LOW_LEVEL);
    esp_sleep_enable_gpio_wakeup();
}

void enable_both_wakeups(gpio_num_t pin, uint64_t sleep_us) {
    enable_gpio_wakeup(pin);
    esp_sleep_enable_timer_wakeup(sleep_us);
}

bool was_wakeup_from_gpio() {
    return esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_GPIO;
}

void print_wakeup_reason() {
    esp_sleep_wakeup_cause_t wakeup_reason = esp_sleep_get_wakeup_cause();
    switch (wakeup_reason) {
        case ESP_SLEEP_WAKEUP_EXT0:
            ESP_LOGI(TAG, "Wakeup caused by external signal using RTC_IO");
            break;
        case ESP_SLEEP_WAKEUP_GPIO:
            ESP_LOGI(TAG, "Wakeup caused by GPIO");
            break;
        case ESP_SLEEP_WAKEUP_TIMER:
            ESP_LOGI(TAG, "Wakeup caused by timer");
            break;
        default:
            ESP_LOGI(TAG, "Wakeup not caused by external events: %d", wakeup_reason);
            break;
    }
}

bool should_perform_ble_scan(uint64_t interval_min) {
    uint64_t now = esp_timer_get_time(); // µs
    uint64_t delta = now - last_ble_poll_us;
    if (delta >= interval_min * 60ULL * 1000000ULL) {
        last_ble_poll_us = now;
        return true;
    }
    return false;
}

bool should_attempt_wifi_upload(uint64_t interval_min) {
    uint64_t now = esp_timer_get_time(); // µs
    uint64_t delta = now - last_wifi_attempt_us;
    if (delta >= interval_min * 60ULL * 1000000ULL) {
        last_wifi_attempt_us = now;
        return true;
    }
    return false;
}

void prepare_for_deep_sleep(uint64_t sleep_us, gpio_num_t wake_gpio) {
    ESP_LOGI(TAG, "Entering deep sleep...");
    print_wakeup_reason();
    init_power_management();
    enable_both_wakeups(wake_gpio, sleep_us);
    enter_deep_sleep_with_timer(sleep_us);
}

