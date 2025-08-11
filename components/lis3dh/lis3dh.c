#include "lis3dh.h"
#include "esp_log.h"
#include "esp_check.h"
#include "freertos/FreeRTOS.h"

static const char *TAG = "lis3dh";

static esp_err_t write_reg(lis3dh_t *dev, uint8_t reg, uint8_t val) {
    uint8_t buf[2] = { reg, val };
    return i2c_master_write_to_device(dev->i2c_num, dev->i2c_addr, buf, 2, pdMS_TO_TICKS(100));
}

static esp_err_t read_reg(lis3dh_t *dev, uint8_t reg, uint8_t *val) {
    return i2c_master_write_read_device(dev->i2c_num, dev->i2c_addr, &reg, 1, val, 1, pdMS_TO_TICKS(100));
}

static esp_err_t read_regs(lis3dh_t *dev, uint8_t start_reg, uint8_t *data, size_t len) {
    uint8_t reg = start_reg | 0x80; // auto-increment
    return i2c_master_write_read_device(dev->i2c_num, dev->i2c_addr, &reg, 1, data, len, pdMS_TO_TICKS(100));
}

esp_err_t lis3dh_read_whoami(lis3dh_t *dev, uint8_t *val) {
    ESP_RETURN_ON_FALSE(dev && val, ESP_ERR_INVALID_ARG, TAG, "null");
    return read_reg(dev, LIS3DH_REG_WHO_AM_I, val);
}

esp_err_t lis3dh_init(lis3dh_t *dev) {
    ESP_RETURN_ON_FALSE(dev, ESP_ERR_INVALID_ARG, TAG, "null dev");

    uint8_t who = 0;
    ESP_RETURN_ON_ERROR(lis3dh_read_whoami(dev, &who), TAG, "whoami read failed");
    if (who != LIS3DH_WHO_AM_I_VAL) {
        ESP_LOGE(TAG, "unexpected WHO_AM_I=0x%02X", who);
        return ESP_FAIL;
    }

    // 100 Hz ODR, all axes enable: ODR=100Hz (0101), X/Y/Z enable (1)
    ESP_RETURN_ON_ERROR(write_reg(dev, LIS3DH_REG_CTRL1, 0x57), TAG, "CTRL1");
    // INT1: DRDY on INT1 (optional)
    ESP_RETURN_ON_ERROR(write_reg(dev, LIS3DH_REG_CTRL3, 0x10), TAG, "CTRL3");
    // High-resolution, ±2g, BDU=1
    ESP_RETURN_ON_ERROR(write_reg(dev, LIS3DH_REG_CTRL4, 0x88), TAG, "CTRL4");

    ESP_LOGI(TAG, "initialized");
    return ESP_OK;
}

esp_err_t lis3dh_read_xyz_raw(lis3dh_t *dev, int16_t *x, int16_t *y, int16_t *z) {
    ESP_RETURN_ON_FALSE(dev && x && y && z, ESP_ERR_INVALID_ARG, TAG, "null");
    uint8_t raw[6] = {0};
    esp_err_t err = read_regs(dev, LIS3DH_REG_OUT_X_L, raw, sizeof raw);
    if (err != ESP_OK) return err;
    // 12-bit left-justified in 16-bit register when HR=1; shift >>4 to align
    *x = (int16_t)((raw[1] << 8) | raw[0]) >> 4;
    *y = (int16_t)((raw[3] << 8) | raw[2]) >> 4;
    *z = (int16_t)((raw[5] << 8) | raw[4]) >> 4;
    return ESP_OK;
}

