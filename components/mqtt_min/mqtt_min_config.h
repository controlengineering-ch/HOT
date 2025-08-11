#include "mqtt_min_config.h"
#include "esp_event.h"
#include "esp_log.h"
#include "mqtt_client.h"
#include "esp_timer.h"
#include <stdio.h>

static const char *TAG = "mqtt_min";

static esp_mqtt_client_handle_t client = NULL;
static int64_t last_publish_ms = 0;

static void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data) {
    esp_mqtt_event_handle_t event = event_data;
    switch (event_id) {
        case MQTT_EVENT_CONNECTED:
            ESP_LOGI(TAG, "Connected");
            break;
        case MQTT_EVENT_DISCONNECTED:
            ESP_LOGW(TAG, "Disconnected");
            break;
        default: break;
    }
}

void mqtt_min_start(void) {
    esp_mqtt_client_config_t cfg = {
        .broker.address.uri = RADON_MQTT_BROKER_URI,
        .credentials = {
            .username = RADON_MQTT_USERNAME,
            .authentication.password = RADON_MQTT_PASSWORD
        },
        .session = {
            .last_will = {
                .topic = RADON_MQTT_TOPIC_BASE "/status",
                .msg = "offline",
                .qos = RADON_MQTT_QOS,
                .retain = 1
            }
        },
        .network = {
            .reconnect_timeout_ms = 2000,
        },
        .credentials.client_id = RADON_MQTT_CLIENT_ID,
        .session.keepalive = RADON_MQTT_KEEPALIVE,
    };
    client = esp_mqtt_client_init(&cfg);
    esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt_event_handler, NULL);
    esp_mqtt_client_start(client);
}

static int publish(const char *topic, const char *payload, int qos, int retain) {
    if (!client) return -1;
    return esp_mqtt_client_publish(client, topic, payload, 0, qos, retain);
}

// Call periodically, e.g., from a timer or main loop
void mqtt_min_tick(const char *radon_value_str) {
    int64_t now_ms = esp_timer_get_time() / 1000;
    if (now_ms - last_publish_ms < RADON_MQTT_KEEPALIVE * 1000) return;
    last_publish_ms = now_ms;

    char topic[128];
    snprintf(topic, sizeof topic, "%s/radon", RADON_MQTT_TOPIC_BASE);
    publish(topic, radon_value_str, RADON_MQTT_QOS, 0);

    publish(RADON_MQTT_TOPIC_BASE "/status", "online", RADON_MQTT_QOS, 1);
}

