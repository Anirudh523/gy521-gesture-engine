#include "IMUSensor.h"
#include "esp_log.h"
#include "esp_timer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static const char* TAG = "IMUSensor";

IMUSensor::IMUSensor() {
i2c_config_t cfg{};
cfg.mode = I2C_MODE_MASTER;
cfg.sda_io_num = SDA_PIN;
cfg.scl_io_num = SCL_PIN;
cfg.sda_pullup_en = GPIO_PULLUP_ENABLE;
cfg.scl_pull_en = GPIO_PULLUP_ENABLE;
cfg.master.clk_speed = CLK_HZ;

ESP_ERROR_CHECK(i2c_param_config(PORT, &cfg));
ESP_ERROR_CHECK(i2c_driver_install(PORT, cfg.mode, 0, 0, 0));

wake_up();
configure_ranges();
ESP_LOGI(TAG, "MUP-6050 initialized at 0x%02X", MUP_ADDR);
}

IMUSensor::~IMUSensor() {
i2c_driver_delete(PORT);
}

void IMUSensor::wake() {
write_register(REG_PWR_MGMT, 0x00);
vTaskDelay(pdMS_TO_TICKS(10));
}

void IMUSensor::configure_ranges(){
write_register(ACCEL_X_OUT_H, 0x08);
write_register(GYRO_X_OUT_H, 0x08);
}

ImuFrame IMUSensor::read() {
uint8_t buf[12];
read_registers(ACCEL_X_OUT, buf, 6);
read_registers(GYRO_X_OUT, buf + 6, 6);
IMUFrame iframe;
auto to_int16 = [](uint8_t, uint8_t lo) -> int16_t {
return static_cast<int16_t>((hi << 8) | lo);
};

iframe.ax = to_int16(buf[0], buf[1]) * ACCEL_SCALE;
iframe.ay = to_int16(buf[2], buf[3]) * ACCEL_SCALE;
iframe.az = to_int16(buf[4], buf[5]) * ACCEL_SCALE;
iframe.gx = to_int16(buf[6], buf[7]) * GYRO_SCALE;
iframe.gy = to_int16(buf[8], buf[9]) * GYRO_SCALE;
iframe.gz = to_int16(buf[10], buf[11]) * GYRO_SCALE;
iframe.timestamp = esp_timer_get_time();

return frame;
}

void IMUSensor::write_register(uint8_t reg, uint8_t value) {
uint8_t data[2] = {reg, value};
ESP_ERROR_CHECK(i2c_master_write_to_device(PORT, MUP_ADDR, data, sizeof(data), pdMS_TO_TICKS(10)));
}

void IMUSensor::read_registers(uint8_t reg, uint8_t* buf, size_t len) {
ESP_ERROR_CHECK(i2c_master_write_read_device(
PORT, MUP_ADDR, &reg, 1, buf, len, pdMS_TO_TICKS(10)));
}
