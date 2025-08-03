#include "azure_upload.h"
#include <esp_log.h>
#include <esp_wifi.h>
#include <esp_event.h>
#include <nvs_flash.h>
#include <esp_http_client.h>

static const char *TAG = "Azure";

static const char *ssid = "YourSSID";
static const char *password = "YourPassword";
static const char *azure_endpoint = "https://your-azure-endpoint/api/upload";

void init_wifi()
{
    ESP_LOGI(TAG, "Yhdistetään WiFi-verkkoon...");

    esp_netif_init();
    esp_event_loop_create_default();
    esp_netif_create_default_wifi_sta();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&cfg);

    wifi_config_t wifi_config = {};
    strncpy((char *)wifi_config.sta.ssid, ssid, sizeof(wifi_config.sta.ssid));
    strncpy((char *)wifi_config.sta.password, password, sizeof(wifi_config.sta.password));

    esp_wifi_set_mode(WIFI_MODE_STA);
    esp_wifi_set_config(WIFI_IF_STA, &wifi_config);
    esp_wifi_start();

    esp_wifi_connect();
}

bool is_wifi_connected()
{
    wifi_ap_record_t ap_info;
    return esp_wifi_sta_get_ap_info(&ap_info) == ESP_OK;
}

void upload_data_to_cloud(const SensorData &data)
{
    if (!is_wifi_connected()) {
        ESP_LOGW(TAG, "Ei WiFi-yhteyttä – pilvilähetys ohitetaan.");
        return;
    }

    char post_data[256];
    snprintf(post_data, sizeof(post_data),
             "{\"sensor\":\"%s\",\"radon\":%d,\"timestamp\":%u}",
             data.identifier.c_str(), data.radon, data.timestamp);

    esp_http_client_config_t config = {
        .url = azure_endpoint,
        .method = HTTP_METHOD_POST,
    };

    esp_http_client_handle_t client = esp_http_client_init(&config);
    esp_http_client_set_header(client, "Content-Type", "application/json");
    esp_http_client_set_post_field(client, post_data, strlen(post_data));

    esp_err_t err = esp_http_client_perform(client);
    if (err == ESP_OK) {
        ESP_LOGI(TAG, "Pilvilähetys onnistui. HTTP status = %d",
                 esp_http_client_get_status_code(client));
    } else {
        ESP_LOGE(TAG, "Pilvilähetys epäonnistui: %s", esp_err_to_name(err));
    }

    esp_http_client_cleanup(client);
}

