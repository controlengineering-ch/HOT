#include "radon_alert.h"
#include <ctime>

static inline float down_thresh(float base, float hysteresis_pct) {
    if (hysteresis_pct <= 0.0f) return base;                // ei hystereesiä
    if (hysteresis_pct >= 100.0f) return 0.0f;              // ääripää: pakottaa alas heti
    return base * (1.0f - (hysteresis_pct / 100.0f));
}

const char* alert_level_str(AlertLevel lvl) {
    switch (lvl) {
        case AlertLevel::Normal:   return "NORMAL";
        case AlertLevel::Warning:  return "WARNING";
        case AlertLevel::Critical: return "CRITICAL";
        default:                   return "UNKNOWN";
    }
}

RadonAlert evaluate_radon(float value_bq_m3,
                          std::time_t sample_ts,
                          const RadonThresholds& t,
                          AlertLevel prev_level)
{
    // 1) Eskalointi (ylöspäin) ilman alas-hysteresisrajoitteita
    if (value_bq_m3 >= t.crit_bq_m3) {
        return { AlertLevel::Critical, value_bq_m3, sample_ts };
    }
    if (value_bq_m3 >= t.warn_bq_m3) {
        // Jos oltiin kriittinen, pysy kriittisenä kunnes alitetaan kriittinen ALAS-kynnys
        float crit_down = down_thresh(t.crit_bq_m3, t.hysteresis_pct);
        if (prev_level == AlertLevel::Critical && value_bq_m3 >= crit_down) {
            return { AlertLevel::Critical, value_bq_m3, sample_ts };
        }
        return { AlertLevel::Warning, value_bq_m3, sample_ts };
    }

    // 2) De-eskalointi (alas) hystereesillä
    //    - Pysytään varoituksessa kunnes alitetaan warn_down
    float warn_down = down_thresh(t.warn_bq_m3, t.hysteresis_pct);
    if (prev_level == AlertLevel::Warning && value_bq_m3 >= warn_down) {
        return { AlertLevel::Warning, value_bq_m3, sample_ts };
    }

    // 3) Muuten NORMAL
    return { AlertLevel::Normal, value_bq_m3, sample_ts };
}

