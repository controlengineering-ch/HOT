#pragma once
#include <cstdint>
#include <string>

struct SensorData {
    std::string identifier;
    int         radon{0};
    uint32_t    timestamp{0};
    bool        valid{false};
};

void init_ble();
SensorData scan_for_waveplus();

