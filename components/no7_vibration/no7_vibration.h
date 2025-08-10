#pragma once
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "driver/gpio.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    NO7_EVT_NONE = 0,
    NO7_EVT_RISE,
    NO7_EVT_FALL
} no7_event_type_t;

typedef struct {
    no7_event_type_t type;
    gpio_num_t       gpio;   // pin
    uint32_t         level;  // 0/1
    uint32_t         ts_ms;  // timestamp (ms)
} no7_event_t;

// Public event queue. You may assign your own queue before init.
extern QueueHandle_t no7_event_queue;

void no7_init(gpio_num_t pin);
void no7_deinit(void);

#ifdef __cplusplus
}
#endif

