#pragma once

#include "driver/gpio.h"
#include "freertos/queue.h"

typedef enum {
    NO7_EVENT_TRIGGERED
} no7_event_type_t;

typedef struct {
    no7_event_type_t type;
    int64_t timestamp; // µs timestamp
} no7_event_t;

extern QueueHandle_t no7_event_queue;

esp_err_t no7_init(gpio_num_t gpio);
