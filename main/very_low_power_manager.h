#ifndef VERY_LOW_POWER_MANAGER_H
#define VERY_LOW_POWER_MANAGER_H

#include "esp_sleep.h"
#include "driver/gpio.h"
#include "esp_log.h"

#ifdef __cplusplus
extern "C" {
#endif

void init_power_management();
void enter_deep_sleep_with_timer(uint64_t sleep_us);
void enable_gpio_wakeup(gpio_num_t pin);
void enable_both_wakeups(gpio_num_t pin, uint64_t sleep_us);
bool was_wakeup_from_gpio();
void print_wakeup_reason();
bool should_perform_ble_scan(uint64_t interval_min);
bool should_attempt_wifi_upload(uint64_t interval_min);
void prepare_for_deep_sleep(uint64_t sleep_us, gpio_num_t wake_gpio);

#ifdef __cplusplus
}
#endif

#endif // VERY_LOW_POWER_MANAGER_H

