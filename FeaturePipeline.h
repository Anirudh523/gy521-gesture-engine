struct FeatureVector{
	float rms[6];
	float mean[6];
	float zcr[6];
	bool read;
}

template<size_t WindowSize>
class FeaturePipeline{
private:
	FeatureVector result_;
	IMUFrame* buffer[WindowSize];
	size_t index_;
	void compute();
public:
	FeaturePipeline();
	void push();
	FeatureVector getFeatures();
	bool isReady();
	void reset();
}
