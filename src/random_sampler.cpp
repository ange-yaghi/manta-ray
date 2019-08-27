#include "../include/random_sampler.h"

manta::RandomSampler::RandomSampler() {
    /* void */
}

manta::RandomSampler::~RandomSampler() {
    /* void */
}

void manta::RandomSampler::generateSamples(int sampleCount, math::Vector2 *target) const {
    for (int s = 0; s < sampleCount; s++) {
        math::real r1 = math::uniformRandom();
        math::real r2 = math::uniformRandom();

        target[s] = math::Vector2(r1, r2);
    }
}

int manta::RandomSampler::getTotalSampleCount(int sampleCount) const {
    return sampleCount;
}
