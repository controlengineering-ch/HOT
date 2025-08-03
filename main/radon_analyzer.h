#pragma once
#include "ble_scan.h"

enum AlertLevel {
    ALERT_NONE = 0,
    ALERT_WARN,
    ALERT_DANGER
};

struct RadonAlert {
    AlertLevel level;
    std::string message;
    bool active;
};

RadonAlert analyze_radon(const SensorData &data);
void trigger_alert_visuals(const RadonAlert &alert);

