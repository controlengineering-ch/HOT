#include "no7_vibration.h"
#include "driver/gpio.h"
#include "esp_log.h"

static const char *TAG = "NO7";

static void IRAM_ATTR no7_isr_handler(void* arg)
{
    // Signaali laskevalla reunalla – tärinä havaittu
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    no7_event_t evt = {
        .type = NO7_EVENT_TRIGGERED,
        .timestamp = esp_timer_get_time()
    };

    if (no7_event_queue != NULL) {
        xQueueSendFromISR(no7_event_queue, &evt, &xHigherPriorityTaskWoken);
    }

    if (xHigherPriorityTaskWoken) {
        portYIELD_FROM_ISR();
    }
}

esp_err_t no7_init(gpio_num_t gpio)
{
    gpio_config_t io_conf = {
        .pin_bit_mask = 1ULL << gpio,
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_NEGEDGE  // Laskeva reuna = tärinä havaittu
    };

    ESP_ERROR_CHECK(gpio_config(&io_conf));
    ESP_ERROR_CHECK(gpio_install_isr_service(0));
    ESP_ERROR_CHECK(gpio_isr_handler_add(gpio, no7_isr_handler, (void*) gpio));

    ESP_LOGI(TAG, "NO7 vibration sensor initialized on GPIO %d", gpio);
    return ESP_OK;
}
