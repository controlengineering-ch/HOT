#include "very_low_power_manager.h"
#include "esp_log.h"
#include "esp_sleep.h"
#include "esp_wifi.h"
#include "esp_bt.h"
// jos käytät LVGL-näyttöä ja haluat sen pois päältä ennen sleepiä, lisää oma funkkari:
// #include "lvgl_gui.h"

static const char* TAG = "vlp";

void init_power_management() {
    // laita tänne DVFS/PM-init jos käytät
    ESP_LOGI(TAG, "Power management initialized");
}

void enter_deep_sleep_with_timer(uint64_t sleep_us) {
    esp_deep_sleep(sleep_us);
}

void enable_gpio_wakeup(gpio_num_t pin) {
    esp_sleep_enable_ext0_wakeup(pin, 0); // herää kun pinni menee LOW (muuta tarpeen mukaan)
}

void enable_both_wakeups(gpio_num_t pin, uint64_t sleep_us) {
    enable_gpio_wakeup(pin);
    esp_sleep_enable_timer_wakeup(sleep_us);
}

bool was_wakeup_from_gpio() {
    return esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_EXT0;
}

void print_wakeup_reason() {
    esp_sleep_wakeup_cause_t cause = esp_sleep_get_wakeup_cause();
    ESP_LOGI(TAG, "Wakeup cause: %d", (int)cause);
}

bool should_perform_ble_scan(uint64_t /*interval_min*/) {
    // palauta haluamasi logiikka (esim. NVS-ajastin)
    return false;
}

bool should_attempt_wifi_upload(uint64_t /*interval_min*/) {
    // palauta haluamasi logiikka
    return false;
}

void prepare_for_deep_sleep(uint64_t sleep_us, gpio_num_t wake_gpio) {
    // jos sinulla on näyttö: sammuta taustavalo jne.
    // lvgl_gui_display_off();  // jos toteutettu
    // gpio_set_level(GPIO_NUM_0, 0); // esimerkkiledi pois

    esp_wifi_stop();
    esp_bt_controller_disable();

    enable_both_wakeups(wake_gpio, sleep_us);
    ESP_LOGI(TAG, "Entering deep sleep for %llu us", (unsigned long long)sleep_us);
    enter_deep_sleep_with_timer(sleep_us);
}
s
