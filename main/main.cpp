#include "IMUSensor.h"
#include "InferenceEngine.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

static const char* TAG = "main";
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
    InferenceEngine engine;
    IMUFrame frame;
    while (true) {
        xQueueReceive(frameQueue, &frame, portMAX_DELAY);
        engine.push(frame);
        if (engine.isReady()) {
            auto result = engine.classify();
            xQueueSend(resultQueue, &result, portMAX_DELAY);
            engine.reset();
        }
    }
}

void outputTask(void* pvParameters) {
    InferenceResult result;
    while (true) {
        xQueueReceive(resultQueue, &result, portMAX_DELAY);
	if(result.valid){
		ESP_LOGI(TAG, "Gesture: %s (%.1f%%)",
                result.label, result.confidence * 100.0f);
    	}
    }
}

extern "C" void app_main() {
    frameQueue = xQueueCreate(10, sizeof(IMUFrame));
    resultQueue = xQueueCreate(5, sizeof(InferenceResult));

    xTaskCreate(sensorTask, "sensor_task", 4096, NULL, 5, NULL);
    xTaskCreate(pipelineTask, "pipeline_task", 8192, NULL, 4, NULL);
    xTaskCreate(outputTask, "output_task", 4096, NULL, 3, NULL);
}
