#include <simple_sampler.h>

manta::SimpleSampler::SimpleSampler() {
}

manta::SimpleSampler::~SimpleSampler() {
}

void manta::SimpleSampler::generateSamples(int sampleCount, math::Vector *target) const {
	for (int i = 0; i < sampleCount; i++) {
		target[i] = math::constants::Zero;
	}
}

int manta::SimpleSampler::getTotalSampleCount(int sampleCount) const {
	return sampleCount;
}
