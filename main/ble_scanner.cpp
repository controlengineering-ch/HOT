#include "ble_scanner.h"
#include <iostream>

// Olemas oleva scan_for_waveplus() pysyy ennallaan...

bool connect_to_sensor(const std::string& mac_address) {
    std::cout << "[BLE] Connecting to sensor " << mac_address << "...\n";
    // TODO: Actual BLE connection logic
    return true;  // Stub: Always succeed
}

int read_radon_value(const std::string& mac_address) {
    std::cout << "[BLE] Reading radon value from " << mac_address << "...\n";
    // TODO: Replace with actual sensor read logic
    return 123;  // Stub: Fixed dummy value
}

std::string read_timestamp(const std::string& mac_address) {
    std::cout << "[BLE] Reading timestamp from " << mac_address << "...\n";
    // TODO: Replace with actual timestamp logic
    return "2025-08-05T22:35:00";  // Stub: Dummy timestamp
}

