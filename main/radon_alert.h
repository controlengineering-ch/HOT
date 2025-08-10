#pragma once
#include <ctime>

// Hälytystasot radonille
enum class AlertLevel {
    Normal,
    Warning,
    Critical
};

// Radonhälytyksen kynnysarvot ja hystereesi
struct RadonThresholds {
    float warn_bq_m3;       // Esim. 150 Bq/m³
    float crit_bq_m3;       // Esim. 220 Bq/m³
    float hysteresis_pct;   // Esim. 10 (%), estää "fläppäyksen"
};

// Radonhälytyksen data
struct RadonAlert {
    AlertLevel level;
    float value_bq_m3;
    std::time_t sample_ts;
};

// Arvioi radon-arvo ja palauttaa hälytysrakenteen
RadonAlert evaluate_radon(float value_bq_m3,
                          std::time_t sample_ts,
                          const RadonThresholds& t,
                          AlertLevel prev_level = AlertLevel::Normal);

// Palauttaa hälytystason tekstinä
const char* alert_level_str(AlertLevel lvl);

