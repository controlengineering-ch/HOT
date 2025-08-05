#include "radon_analyzer.h"
#include "sensor_data.h"
#include "radon_alert.h"
**#include "driver/gpio.h"**  

// 🆕 Tarvitaan GPIO_PIN määrityksiin ja gpio_set_level

void analyze_radon_data(const SensorData &data) {
    RadonAlert alert;

    if (data.radon >= 220) {
        alert.level = AlertLevel::CRITICAL;
        alert.message = "CRITICAL: Radon level too high!";
    } else if (data.radon >= 150) {
        alert.level = AlertLevel::WARNING;
        alert.message = "WARNING: Elevated radon level.";
    } else {
        alert.level = AlertLevel::NORMAL;
        alert.message = "Radon level normal.";
    }

    trigger_alert_visuals(alert);
}

void trigger_alert_visuals(const RadonAlert &alert) {
    // Placeholder: visualize alert with LEDs or GUI changes
    if (alert.level == AlertLevel::CRITICAL) {
        **gpio_set_level(GPIO_NUM_2, 1);**  // Esimerkkipinni
        // Add buzzer or other indicator logic
    } else if (alert.level == AlertLevel::WARNING) {
        **gpio_set_level(GPIO_NUM_2, 1);**
    } else {
        **gpio_set_level(GPIO_NUM_2, 0);**
    }
}

