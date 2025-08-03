#include "control_data.h"

// Oletusarvot
static int polling_interval_ms = 30000;       // 30 s
static int threshold_warn = 150;              // keltainen hälytys
static int threshold_danger = 220;            // punainen hälytys

void set_control_params(int polling_ms, int warn, int danger)
{
    polling_interval_ms = polling_ms;
    threshold_warn = warn;
    threshold_danger = danger;
}

int get_polling_interval()
{
    return polling_interval_ms;
}

int get_warn_threshold()
{
    return threshold_warn;
}

int get_danger_threshold()
{
    return threshold_danger;
}

