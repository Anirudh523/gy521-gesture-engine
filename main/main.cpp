#include "IMUSensor.h"
#include "FeaturePipeline.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static const char* TAG = "main";

extern "C" void app_main() {

IMUSensor imu; 
FeaturePipeline<50> pipeline;


while (true) {

auto frame = imu.read();
pipeline.push(frame);

if(pipeline.isReady()){
	auto features = pipeline.getFeatures();
	ESP_LOGI(TAG, "rms_ax=%.3f mean_ax=%.3f zcr_ax=%.3f", features.rms[0], features.mean[0], features.zcr[0]);
	pipeline.reset();
}
vTaskDelay(pdMS_TO_TICKS(100));
}
}
