#pragma once

#include <string>
#include "sensor_data.h"

SensorData scan_for_waveplus();

// Stub-funktioiden julistukset:
bool connect_to_sensor(const std::string& mac_address);
int read_radon_value(const std::string& mac_address);
std::string read_timestamp(const std::string& mac_address);

