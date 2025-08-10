#include "ble_sensor.h"
#include "esp_log.h"
#include "esp_timer.h"

static const char* TAG = "BLE_SENSOR";

bool connect_to_sensor(const char* mac)
{
    ESP_LOGI(TAG, "(stub) connect_to_sensor: %s", mac ? mac : "(null)");
    // TODO: oikea BLE-yhteys (NimBLE), palauta true jos onnistuu
    return true;
}

float read_radon_value(const char* mac)
{
    ESP_LOGI(TAG, "(stub) read_radon_value from %s", mac ? mac : "(null)");
    // TODO: lue arvo BLE:llä — palautetaan testiarvo
    return 50.0f;
}

uint64_t read_timestamp(const char* mac)
{
    (void)mac;
    // Palautetaan nykyhetki millisekunteina
    return (uint64_t)(esp_timer_get_time() / 1000ULL);
}

