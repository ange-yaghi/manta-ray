#include "../include/sampler.h"

#include <float.h>

manta::Sampler::Sampler() {
    m_currentPixelSample = 0;
    m_samplesPerPixel = 0;

    m_samplesPerPixelInput = nullptr;
}

manta::Sampler::~Sampler() {
    /* void */
}

void manta::Sampler::startPixelSession() {
    m_currentPixelSample = 0;
}

bool manta::Sampler::startNextSample() {
    return ++m_currentPixelSample != m_samplesPerPixel;
}

manta::math::real manta::Sampler::uniformRandom() {
    uint32_t r = m_rng();
    math::real f = r * 0x1p-32f;

    if (f > 0.9999f) f = 0.9999f;

    return f;
}

uint32_t manta::Sampler::uniformRandomInt(uint32_t range) {
    uint32_t threshold = (~range + 1u) % range;
    while (true) {
        uint32_t r = m_rng();
        if (r >= threshold) return r % range;
    }
}

void manta::Sampler::seed(unsigned int seed) {
    m_rng.seed(seed);
}

void manta::Sampler::_evaluate() {
    m_output.setReference(this);

    piranha::native_int samples;
    m_samplesPerPixelInput->fullCompute((void *)&samples);

    m_samplesPerPixel = (int)samples;
}

void manta::Sampler::registerInputs() {
    registerInput(&m_samplesPerPixelInput, "samples");
}
