#pragma once
#include <cstdint>   // <-- tämä puuttui
#include <string>

struct SensorData {
    std::string identifier;
    int         radon;
    uint32_t    timestamp;
    bool        valid;
};

void init_ble();
SensorData scan_for_waveplus();
