#include "IMUSensor.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static const char* TAG = "main";

extern "C" void app_main() {
IMUSensor imu;

while (true) {
auto frame = imu.read();
ESP_LOGI(TAG, "ax=%.2f ay=%.2f az=%.2f | gx=%.2f gy=%.2f gz=%.2f",
            frame.ax, frame.ay, frame.az,
            frame.gx, frame.gy, frame.gz);
        vTaskDelay(pdMS_TO_TICKS(100));

}
}
