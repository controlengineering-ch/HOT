// azure_upload.cpp
#include <cstdio>
#include <cstring>
#include <string>
#include <inttypes.h>   // PRIu32

extern "C" {
#include "esp_log.h"
#include "esp_err.h"
#include "esp_http_client.h"
#include "esp_crt_bundle.h"   // muista PRIV_REQUIRES: mbedtls
}

#if __has_include("ble_scanner.h")
  #include "ble_scanner.h"    // SensorData
#else
  #include "ble_scan.h"
#endif

#ifndef ENDPOINT_URL
#define ENDPOINT_URL "https://example.com/ingest"   // vaihda omaan
#endif

static const char *TAG = "AZURE_UPLOAD";

void upload_data_to_cloud(const SensorData &data)
{
    if (!data.valid) {
        ESP_LOGW(TAG, "SensorData ei validi, ei lähetetä.");
        return;
    }

    char payload[192];
    int n = std::snprintf(
        payload, sizeof(payload),
        "{\"sensor\":\"%s\",\"radon\":%d,\"timestamp\":%" PRIu32 "}",
        data.identifier.c_str(), data.radon, static_cast<uint32_t>(data.timestamp));

    if (n < 0 || n >= static_cast<int>(sizeof(payload))) {
        ESP_LOGE(TAG, "JSON payload ylipitkä (%d bytes), ei lähetetä.", n);
        return;
    }

    esp_http_client_config_t cfg = {};
    cfg.url               = ENDPOINT_URL;
    cfg.method            = HTTP_METHOD_POST;
    cfg.transport_type    = HTTP_TRANSPORT_OVER_SSL;
    cfg.crt_bundle_attach = esp_crt_bundle_attach;   // käyttää IDF:n CA-bundlea
    cfg.timeout_ms        = 8000;

    esp_http_client_handle_t client = esp_http_client_init(&cfg);
    if (!client) {
        ESP_LOGE(TAG, "esp_http_client_init epäonnistui");
        return;
    }

    esp_http_client_set_header(client, "Content-Type", "application/json");
    esp_http_client_set_post_field(client, payload, std::strlen(payload));

    esp_err_t err = esp_http_client_perform(client);
    if (err == ESP_OK) {
        int status = esp_http_client_get_status_code(client);
        int clen   = esp_http_client_get_content_length(client);
        ESP_LOGI(TAG, "POST OK: status=%d, len=%d", status, clen);
    } else {
        ESP_LOGE(TAG, "POST epäonnistui: %s", esp_err_to_name(err));
    }

    esp_http_client_cleanup(client);
}

