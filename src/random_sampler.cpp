#include "../include/random_sampler.h"

manta::RandomSampler::RandomSampler() {
    /* void */
}

manta::RandomSampler::~RandomSampler() {
    /* void */
}

manta::math::real manta::RandomSampler::generate1d() {
    return uniformRandom();
}

manta::math::Vector2 manta::RandomSampler::generate2d() {
    return math::Vector2(
        uniformRandom(),
        uniformRandom());
}

manta::Sampler *manta::RandomSampler::clone() const {
    RandomSampler *newSampler = new RandomSampler;
    newSampler->setSamplesPerPixel(m_samplesPerPixel);
    
    return newSampler;
}
