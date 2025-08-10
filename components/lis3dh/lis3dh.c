#include "lis3dh.h"
#include "esp_log.h"

static const char *TAG = "lis3dh_stub";

static lis3dh_cfg_t s_cfg;
static bool s_inited = false;

esp_err_t lis3dh_init(const lis3dh_cfg_t *cfg)
{
    if (!cfg) return ESP_ERR_INVALID_ARG;
    s_cfg = *cfg;

    if (s_cfg.install_driver) {
        i2c_config_t c = {
            .mode = I2C_MODE_MASTER,
            .sda_io_num = s_cfg.sda_io,
            .scl_io_num = s_cfg.scl_io,
            .sda_pullup_en = GPIO_PULLUP_ENABLE,
            .scl_pullup_en = GPIO_PULLUP_ENABLE,
            .master.clk_speed = s_cfg.clk_hz ? s_cfg.clk_hz : 400000
        };
        ESP_ERROR_CHECK(i2c_param_config(s_cfg.port, &c));
        ESP_ERROR_CHECK(i2c_driver_install(s_cfg.port, I2C_MODE_MASTER, 0, 0, 0));
    }

    s_inited = true;
    ESP_LOGI(TAG, "initialized (stub), addr=0x%02X", s_cfg.addr);
    return ESP_OK;
}

esp_err_t lis3dh_read_whoami(uint8_t *whoami)
{
    if (!whoami) return ESP_ERR_INVALID_ARG;
    if (!s_inited) return ESP_ERR_INVALID_STATE;
    *whoami = 0x33; // datasheetin arvo – stub
    return ESP_OK;
}

esp_err_t lis3dh_read_accel_mg(int16_t *ax_mg, int16_t *ay_mg, int16_t *az_mg)
{
    if (!ax_mg || !ay_mg || !az_mg) return ESP_ERR_INVALID_ARG;
    if (!s_inited) return ESP_ERR_INVALID_STATE;
    *ax_mg = 0; *ay_mg = 0; *az_mg = 0; // stub
    return ESP_OK;
}

esp_err_t lis3dh_power_down(i2c_port_t port)
{
    (void)port;
    if (!s_inited) return ESP_ERR_INVALID_STATE;
    // Tarvittaessa: i2c_driver_delete(s_cfg.port);
    s_inited = false;
    ESP_LOGI(TAG, "power down (stub)");
    return ESP_OK;
}

