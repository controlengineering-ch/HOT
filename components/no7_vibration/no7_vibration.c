#include "no7_vibration.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "esp_timer.h"
#include "esp_log.h"
#include <stdatomic.h>
#include <stddef.h>

static const char *TAG = "no7_vibration";

QueueHandle_t no7_event_queue = NULL;

#ifndef GPIO_NUM_NC
#define GPIO_NUM_NC ((gpio_num_t)-1)
#endif

static gpio_num_t  s_pin    = GPIO_NUM_NC;
static atomic_bool s_inited = ATOMIC_VAR_INIT(false);

static void IRAM_ATTR no7_gpio_isr(void *arg)
{
    (void)arg;
    if (!no7_event_queue || s_pin == GPIO_NUM_NC) return;

    int level = gpio_get_level(s_pin);
    no7_event_t evt = {
        .type  = level ? NO7_EVT_RISE : NO7_EVT_FALL,
        .gpio  = s_pin,
        .level = (uint32_t)level,
        .ts_ms = (uint32_t)(esp_timer_get_time() / 1000ULL),
    };

    BaseType_t hpw = pdFALSE;
    xQueueSendFromISR(no7_event_queue, &evt, &hpw);
    if (hpw == pdTRUE) {
        portYIELD_FROM_ISR();
    }
}

void no7_init(gpio_num_t pin)
{
    if (atomic_load(&s_inited)) {
        ESP_LOGW(TAG, "already initialized");
        return;
    }

    s_pin = pin;

    // Create default queue if none provided
    if (!no7_event_queue) {
        no7_event_queue = xQueueCreate(16, sizeof(no7_event_t));
        if (!no7_event_queue) {
            ESP_LOGE(TAG, "queue create failed");
            s_pin = GPIO_NUM_NC;
            return;
        }
    }

    gpio_config_t cfg = {
        .pin_bit_mask = 1ULL << s_pin,
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_ANYEDGE,
    };
    gpio_config(&cfg);

    // Install ISR service (idempotent)
    esp_err_t err = gpio_install_isr_service(0);
    if (err != ESP_OK && err != ESP_ERR_INVALID_STATE) {
        ESP_LOGE(TAG, "gpio_install_isr_service failed: %d", err);
        return;
    }

    gpio_isr_handler_add(s_pin, no7_gpio_isr, NULL);

    atomic_store(&s_inited, true);
    ESP_LOGI(TAG, "initialized on GPIO %d", (int)s_pin);
}

void no7_deinit(void)
{
    if (!atomic_load(&s_inited)) return;

    gpio_isr_handler_remove(s_pin);
    s_pin = GPIO_NUM_NC;
    atomic_store(&s_inited, false);
    ESP_LOGI(TAG, "deinitialized");
}

