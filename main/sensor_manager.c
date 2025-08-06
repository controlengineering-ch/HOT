#include "sensor_manager.h"
#include "no7_vibration.h"
#include "lis3dh.h"
#include "esp_log.h"

#define NO7_GPIO GPIO_NUM_6
#define I2C_PORT I2C_NUM_0
#define I2C_SDA GPIO_NUM_4
#define I2C_SCL GPIO_NUM_5

static const char* TAG = "SENSOR_MGR";

static bool sensor_states[SENSOR_MAX] = {false};
static QueueHandle_t no7_evt_q = NULL;

void sensor_manager_init(void) {
    no7_evt_q = xQueueCreate(10, sizeof(no7_event_t));
}

void sensor_manager_start(sensor_id_t id)
{
    if (sensor_states[id]) return;

    switch (id) {
        case SENSOR_NO7:
            no7_event_queue = no7_evt_q;
            no7_init(NO7_GPIO);
            ESP_LOGI(TAG, "NO7 started.");
            break;
        case SENSOR_NO8:
            lis3dh_init(I2C_PORT, I2C_SDA, I2C_SCL);
            ESP_LOGI(TAG, "NO8 (LIS3DH) started.");
            break;
        case SENSOR_NO1:
            ESP_LOGI(TAG, "NO1 start not yet implemented.");
            break;
        default:
            return;
    }
    sensor_states[id] = true;
}

void sensor_manager_stop(sensor_id_t id)
{
    if (!sensor_states[id]) return;

    switch (id) {
        case SENSOR_NO7:
            ESP_LOGI(TAG, "NO7 stopped.");
            break;
        case SENSOR_NO8:
            lis3dh_power_down(I2C_PORT);
            ESP_LOGI(TAG, "NO8 (LIS3DH) stopped.");
            break;
        case SENSOR_NO1:
            ESP_LOGI(TAG, "NO1 stop not yet implemented.");
            break;
        default:
            return;
    }
    sensor_states[id] = false;
}

void sensor_manager_stop_all(void)
{
    for (int i = 0; i < SENSOR_MAX; i++) {
        sensor_manager_stop((sensor_id_t)i);
    }
}

bool sensor_manager_is_running(sensor_id_t id)
{
    return sensor_states[id];
}

void sensor_manager_print_status(void)
{
    printf("Sensor status:\n");
    printf("  NO7:  %s\n", sensor_states[SENSOR_NO7] ? "RUNNING" : "STOPPED");
    printf("  NO8:  %s\n", sensor_states[SENSOR_NO8] ? "RUNNING" : "STOPPED");
    printf("  NO1:  %s\n", sensor_states[SENSOR_NO1] ? "RUNNING" : "STOPPED");
}
