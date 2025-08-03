#include "ble_scan.h"
#include <esp_log.h>
#include <string>

static const char *TAG = "BLE";

void init_ble()
{
    // Alustetaan BLE (esim. nimetty client/central rooliin)
    ESP_LOGI(TAG, "BLE alustetaan...");
    // Tätä alustusta voi käyttää ESP-IDF BLE GATT-client esimerkistä
}

SensorData scan_for_waveplus()
{
    SensorData result;
    result.valid = false;

    ESP_LOGI(TAG, "Aloitetaan Wave Plus -skannaus...");

    // Esimerkki: kovakoodattu MAC-osoite (voit myöhemmin hakea dynaamisesti)
    std::string sensor_mac = "F0:99:B6:AA:BB:CC";

    // Pseudokoodi: yritetään yhdistää ja hakea arvo
    if (connect_to_sensor(sensor_mac)) {
        result.identifier = sensor_mac;
        result.radon = read_radon_value(sensor_mac);
        result.timestamp = read_timestamp(sensor_mac);
        result.valid = true;
        ESP_LOGI(TAG, "Sensorilta saatu radon-arvo: %d", result.radon);
    } else {
        ESP_LOGW(TAG, "Sensoriin ei saatu yhteyttä.");
    }

    return result;
}

