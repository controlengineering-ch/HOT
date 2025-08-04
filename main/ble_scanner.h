#ifndef BLE_SCANNER_H
#define BLE_SCANNER_H

#include <string>
#include <vector>

struct SensorInfo {
    std::string macAddress;
    int rssi;
};

void startBleScan();
std::vector<SensorInfo> getDiscoveredSensors();

#endif // BLE_SCANNER_H

