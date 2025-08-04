#ifndef VERY_LOW_POWER_MANAGER_H
#define VERY_LOW_POWER_MANAGER_H

#include "esp_sleep.h"
#include "driver/gpio.h"
#include "esp_log.h"

#ifdef __cplusplus
extern "C" {
#endif

**_void init_power_management();_**
**_void enter_deep_sleep_with_timer(uint64_t sleep_us);_**
**_void enable_gpio_wakeup(gpio_num_t pin);_**
**_void enable_both_wakeups(gpio_num_t pin, uint64_t sleep_us);_**
**_bool was_wakeup_from_gpio();_**
**_void print_wakeup_reason();_**

#ifdef __cplusplus
}
#endif

#endif // VERY_LOW_POWER_MANAGER_H

