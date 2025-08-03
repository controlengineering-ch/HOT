#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "lvgl_gui.h"
#include "log_to_sd.h"

static const char* TAG = "MAIN";

extern "C" void app_main() {
    ESP_LOGI(TAG, "RadonSafe v1.0 käynnistyy...");

    // 🟢 Alustetaan SD-kortti
    if (!initSDCard()) {
        ESP_LOGE(TAG, "SD-kortin alustaminen epäonnistui!");
    } else {
        ESP_LOGI(TAG, "SD-kortti alustettu onnistuneesti.");
    }

    // 🖼️ Käynnistä GUI
    lvgl_gui_init();

    // 📝 Kirjoita esimerkkidata logiin
    logSensorData("bt-ABC123", 180.5, 620.0, 21.3, 45.1);

    // 🔄 FreeRTOS looppi
    while (true) {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

