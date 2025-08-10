#pragma once
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// Yksinkertaiset stub-API:t Waveplus/BLE-sensorille.
// Toteuta myöhemmin oikea BLE-logiikka näihin.
bool     connect_to_sensor(const char* mac);
float    read_radon_value(const char* mac);
uint64_t read_timestamp(const char* mac);

#ifdef __cplusplus
}
#endif

