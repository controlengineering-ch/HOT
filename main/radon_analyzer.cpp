#include "radon_analyzer.h"
#include "control_data.h"
#include <esp_log.h>

static const char *TAG = "Analyzer";

RadonAlert analyze_radon(const SensorData &data)
{
    RadonAlert alert;
    alert.level = ALERT_NONE;
    alert.active = false;

    int warn_threshold = get_warn_threshold();
    int danger_threshold = get_danger_threshold();

    ESP_LOGI(TAG, "Analysoidaan radon-arvo: %d", data.radon);

    if (data.radon >= danger_threshold) {
        alert.level = ALERT_DANGER;
        alert.message = "Radon level HI HI";
        alert.active = true;
    } else if (data.radon >= warn_threshold) {
        alert.level = ALERT_WARN;
        alert.message = "Radon level HI";
        alert.active = true;
    }

    return alert;
}

void trigger_alert_visuals(const RadonAlert &alert)
{
    if (alert.level == ALERT_DANGER) {
        // Punainen LED ja vilkku
        gpio_set_level(GPIO_NUM_2, 1);  // Esimerkkipinni
        // Vilkutuslogiikka voisi olla FreeRTOS task
    } else if (alert.level == ALERT_WARN) {
        // Keltainen LED
        gpio_set_level(GPIO_NUM_2, 1);
    }
}

