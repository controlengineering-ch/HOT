#pragma once
#include "driver/i2c.h"
#include "driver/gpio.h"
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

// Default I2C address for LIS3DH (depends on SA0 pin): 0x18 or 0x19
#define LIS3DH_I2C_ADDR_LOW   0x18
#define LIS3DH_I2C_ADDR_HIGH  0x19

// Registers
#define LIS3DH_REG_WHO_AM_I   0x0F
#define LIS3DH_WHO_AM_I_VAL   0x33

#define LIS3DH_REG_CTRL1      0x20
#define LIS3DH_REG_CTRL3      0x22
#define LIS3DH_REG_CTRL4      0x23
#define LIS3DH_REG_INT1_CFG   0x30
#define LIS3DH_REG_INT1_SRC   0x31
#define LIS3DH_REG_OUT_X_L    0x28  // auto-increment to read XYZ

typedef struct {
    i2c_port_t i2c_num;
    uint8_t i2c_addr;     // 0x18 or 0x19
    gpio_num_t int1_gpio; // optional: -1 if unused
} lis3dh_t;

// Public API
esp_err_t lis3dh_init(lis3dh_t *dev);
esp_err_t lis3dh_read_whoami(lis3dh_t *dev, uint8_t *val);
esp_err_t lis3dh_read_xyz_raw(lis3dh_t *dev, int16_t *x, int16_t *y, int16_t *z);

#ifdef __cplusplus
}
#endif

