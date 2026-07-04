#include "IMUSensor.h"
#include "FeaturePipeline.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

QueueHandle_t frameQueue;
QueueHandle_t resultQueue;

void sensorTask(void* pvParameters) {
    IMUSensor imu;
    while (true) {
        auto frame = imu.read();
        xQueueSend(frameQueue, &frame, portMAX_DELAY);
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

void pipelineTask(void* pvParameters) {
    FeaturePipeline<50> pipeline;
    IMUFrame frame;
    while (true) {
        xQueueReceive(frameQueue, &frame, portMAX_DELAY);
        pipeline.push(frame);
        if (pipeline.isReady()) {
            auto features = pipeline.getFeatures();
            xQueueSend(resultQueue, &features, portMAX_DELAY);
            pipeline.reset();
        }
    }
}

void outputTask(void* pvParameters) {
    FeatureVector features;
    while (true) {
        xQueueReceive(resultQueue, &features, portMAX_DELAY);
        ESP_LOGI("output", "rms_ax=%.3f mean_ax=%.3f zcr_ax=%.3f",
            features.rms[0], features.mean[0], features.zcr[0]);
    }
}

extern "C" void app_main() {
    frameQueue = xQueueCreate(10, sizeof(IMUFrame));
    resultQueue = xQueueCreate(5, sizeof(FeatureVector));

    xTaskCreate(sensorTask, "sensor_task", 4096, NULL, 5, NULL);
    xTaskCreate(pipelineTask, "pipeline_task", 4096, NULL, 4, NULL);
    xTaskCreate(outputTask, "output_task", 4096, NULL, 3, NULL);
}
