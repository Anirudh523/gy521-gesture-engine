#pragma once
#include "IMUSensor.h"
#include "edge-impulse-sdk/classifier/ei_run_classifier.h"

struct InferenceResult {
    const char* label;
    float confidence;
    bool valid;
};

class InferenceEngine {
public:
    InferenceEngine() : index_{0} {}

    void push(IMUFrame frame) {
        buffer_[index_++] = frame.ax;
        buffer_[index_++] = frame.ay;
        buffer_[index_++] = frame.az;
        buffer_[index_++] = frame.gx;
        buffer_[index_++] = frame.gy;
        buffer_[index_++] = frame.gz;
    }

    bool isReady() {
        return index_ >= EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE;
    }

    InferenceResult classify() {
        signal_t signal;
        numpy::signal_from_buffer(buffer_, EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE, &signal);

        ei_impulse_result_t result;
        EI_IMPULSE_ERROR err = run_classifier(&signal, &result, false);

        if (err != EI_IMPULSE_OK) {
            return {"unknown", 0.0f, false};
        }

        InferenceResult out{"unknown", 0.0f, true};
        for (size_t i = 0; i < EI_CLASSIFIER_LABEL_COUNT; i++) {
            if (result.classification[i].value > out.confidence) {
                out.confidence = result.classification[i].value;
                out.label = result.classification[i].label;
            }
        }
        return out;
    }

    void reset() {
        index_ = 0;
    }

private:
    float buffer_[EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE];
    size_t index_;
};
