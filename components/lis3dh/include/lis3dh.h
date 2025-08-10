#pragma once
#include "esp_err.h"
#include "driver/i2c.h"
#include "driver/gpio.h"
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

#define LIS3DH_I2C_ADDR 0x18  // vaihtoehtoisesti 0x19

typedef struct {
    i2c_port_t port;          // I2C_NUM_0 / I2C_NUM_1
    uint8_t    addr;          // 0x18 tai 0x19
    gpio_num_t sda_io;
    gpio_num_t scl_io;
    uint32_t   clk_hz;        // esim. 400000
    bool       install_driver;// asennetaanko i2c_driver_install tässä
} lis3dh_cfg_t;

esp_err_t lis3dh_init(const lis3dh_cfg_t *cfg);
esp_err_t lis3dh_read_whoami(uint8_t *whoami);
esp_err_t lis3dh_read_accel_mg(int16_t *ax_mg, int16_t *ay_mg, int16_t *az_mg);
esp_err_t lis3dh_power_down(i2c_port_t port);

#ifdef __cplusplus
}
#endif

