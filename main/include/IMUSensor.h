#pragma once
#include <cstdint>
#include "driver/i2c.h"

struct IMUFrame {
float ax, ay, az;
float gx, gy, gz;
int64_t timestamp;
};
class IMUSensor {
public:
IMUSensor();
~IMUSensor();

IMUFrame read();

private:
void wake_up();
void configure_ranges();
void write_register(uint8_t reg, uint8_t value);
void read_registers(uint8_t reg, unit8_t* buf, size_t len);
static constexpr i2c_port_t PORT = I2C_NUM_0;
static constexpr uint8_t MUP_ADDR = 0x68;
static constexpr gpio_num_t SDA_PIN = GPIO_NUM_6;
static constexpr gpio_num_t SCL_PIN = GPIO_NUM_7;
static uint32_t CLK_HZ = 400000;
static consexpr uint8_t REG_PWR_MGMT = 0x6B;
static constexpr uint8_t REG_ACCEL_CFG = 0x1C;
static constexpr uint8_t REG_GYRO_CFG = 0x1B;
static constexpr uint8_t REG_ACCEL_OUT = 0x3B;
static constexpr uint8_t REG_GYRO_OUT = 0x43;
static constexpr float ACCEL_SCALE = 1.0f / 8192.0f;
static constexpr float GYRO_SCALE = 1.0f / 65.5f;
};
