#include "lvgl_gui.h"
#include "ble_scan.h"
#include "sd_card.h"
#include "radon_analyzer.h"
#include "azure_upload.h"
#include "control_data.h"

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_log.h>

static const char *TAG = "RadonSafeMain";

extern "C" void app_main(void)
{
    ESP_LOGI(TAG, "RadonSafe v1.0 käynnistyy...");

    // 1. Alustetaan SD-kortti
    if (!init_sd_card()) {
        ESP_LOGE(TAG, "SD-kortin alustus epäonnistui!");
    }

    // 2. Alustetaan GUI (LVGL)
    init_lvgl();

    // 3. Alustetaan BLE-skannaus
    init_ble();

    // 4. Alustetaan WiFi ja pilviyhteys
    init_wifi();

    // 5. Ladataan ohjausparametrit SD:ltä
    load_config_from_sd();

    while (true) {
        // 6. Skannataan lähistön Wave Plus -sensorit
        SensorData data = scan_for_waveplus();

        // 7. Jos mitattu -> analysoi ja tallenna
        if (data.valid) {
            save_data_to_sd(data);
            RadonAlert alert = analyze_radon(data);
            update_gui_with_data(data, alert);

            // 8. Tarkista hälytys ja päivitä LED/näyttö
            if (alert.active) {
                trigger_alert_visuals(alert);
            }

            // 9. Jos WiFi-yhteys saatavilla, lähetä data
            if (is_wifi_connected()) {
                upload_data_to_cloud(data);
            }
        }

        // 10. Odota määritelty aika (esim. 30s)
        vTaskDelay(pdMS_TO_TICKS(get_polling_interval()));
    }
}

