#include "../include/simple_sampler.h"

manta::SimpleSampler::SimpleSampler() {
    /* void */
}

manta::SimpleSampler::~SimpleSampler() {
    /* void */
}

void manta::SimpleSampler::generateSamples(int sampleCount, math::Vector2 *target) const {
    for (int i = 0; i < sampleCount; i++) {
        target[i] = math::Vector2((math::real)0.5, (math::real)0.5);
    }
}

int manta::SimpleSampler::getTotalSampleCount(int sampleCount) const {
    return sampleCount;
}
