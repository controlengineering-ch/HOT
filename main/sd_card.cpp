#include "sd_card.h"
#include <esp_log.h>
#include <esp_vfs_fat.h>
#include <nvs_flash.h>
#include <driver/sdspi_host.h>
#include <sdmmc_cmd.h>
#include <cJSON.h>
#include <fstream>

static const char *TAG = "SD";

#define MOUNT_POINT "/sdcard"
#define CONFIG_FILE_PATH "/sdcard/settings/config.json"
#define LOG_FILE_PATH "/sdcard/logs/radon_log.csv"

bool init_sd_card()
{
    ESP_LOGI(TAG, "Alustetaan SD-kortti...");

    esp_vfs_fat_sdmmc_mount_config_t mount_config = {
        .format_if_mount_failed = false,
        .max_files = 5,
        .allocation_unit_size = 16 * 1024
    };

    sdmmc_card_t *card;
    const char mount_point[] = MOUNT_POINT;
    sdmmc_host_t host = SDSPI_HOST_DEFAULT();
    sdspi_slot_config_t slot_config = SDSPI_SLOT_CONFIG_DEFAULT();
    gpio_set_pull_mode(15, GPIO_PULLUP_ONLY); // MISO oletuksena

    esp_err_t ret = esp_vfs_fat_sdspi_mount(mount_point, &host, &slot_config, &mount_config, &card);

    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "❌ SD-kortin mount epäonnistui: %s", esp_err_to_name(ret));
        return false;
    }

    ESP_LOGI(TAG, "✅ SD-kortti valmis.");
    return true;
}

void save_data_to_sd(const SensorData &data)
{
    FILE *f = fopen(LOG_FILE_PATH, "a");
    if (f == nullptr) {
        ESP_LOGE(TAG, "❌ Ei voitu avata logitiedostoa kirjoitusta varten.");
        return;
    }

    fprintf(f, "%s,%d,%u\n", data.identifier.c_str(), data.radon, data.timestamp);
    fclose(f);

    ESP_LOGI(TAG, "✔ Mittaustulos tallennettu: %s", LOG_FILE_PATH);
}

void load_config_from_sd()
{
    FILE *f = fopen(CONFIG_FILE_PATH, "r");
    if (f == nullptr) {
        ESP_LOGW(TAG, "⚠️ Ei löytynyt config.json tiedostoa.");
        return;
    }

    fseek(f, 0, SEEK_END);
    size_t size = ftell(f);
    rewind(f);

    char *content = (char *) malloc(size + 1);
    fread(content, 1, size, f);
    content[size] = 0;
    fclose(f);

    cJSON *json = cJSON_Parse(content);
    if (!json) {
        ESP_LOGE(TAG, "⚠️ JSON parsing epäonnistui.");
        free(content);
        return;
    }

    int polling = cJSON_GetObjectItem(json, "polling_interval")->valueint;
    int warn = cJSON_GetObjectItem(json, "threshold_warn")->valueint;
    int danger = cJSON_GetObjectItem(json, "threshold_danger")->valueint;

    set_control_params(polling, warn, danger);

    ESP_LOGI(TAG, "🧠 Asetukset ladattu: poll=%d, warn=%d, danger=%d", polling, warn, danger);

    cJSON_Delete(json);
    free(content);
}

