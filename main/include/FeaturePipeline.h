#pragma once
#include <cstddef>
#include <cmath>
#include "IMUSensor.h"

struct FeatureVector{
	float rms[6];
	float mean[6];
	float zcr[6];
	bool ready;
};

template<size_t WindowSize>
class FeaturePipeline{
private:
	FeatureVector result_;
	IMUFrame buffer_[WindowSize];
	size_t index_;
	void compute(){
		float axes[WindowSize][6];
		for (size_t i = 0; i < WindowSize; i++) {
        		axes[i][0] = buffer_[i].ax;
        		axes[i][1] = buffer_[i].ay;
      			axes[i][2] = buffer_[i].az;
        		axes[i][3] = buffer_[i].gx;
       			axes[i][4] = buffer_[i].gy;
        		axes[i][5] = buffer_[i].gz;
    		}

    		for (size_t a = 0; a < 6; a++) {
        		float sum_sq   = 0.0f;
        		float sum = 0.0f;
        		int crossings = 0;
        		float prev = axes[0][a];

        		for (size_t i = 0; i < WindowSize; i++) {
            			float val  = axes[i][a];
            			sum_sq += val * val;
            			sum += val;
            			if (i > 0 && prev * val < 0.0f) crossings++;
           			prev = val;
        		}

        		result_.rms[a]  = std::sqrt(sum_sq / WindowSize);
        		result_.mean[a] = sum / WindowSize;
        		result_.zcr[a]  = static_cast<float>(crossings) / (WindowSize - 1);
		}
	}

public:
	FeaturePipeline():result_{}, index_{0}
	{
	}

	void push(IMUFrame iframe){
		buffer_[index_++] = iframe;
		if(index_ == WindowSize){
			compute();
			result_.ready = true;
			index_ = 0;
		}
	}

	FeatureVector getFeatures(){
		return result_;
	}

	bool isReady(){
		return result_.ready;
	}
	void reset(){
		index_ = 0;
		result_.ready = false;
	}
};
