#include "../include/sampler.h"

manta::Sampler::Sampler() {
    /* void */
}

manta::Sampler::~Sampler() {
    /* void */
}

void manta::Sampler::startPixelSession() {
    m_currentPixelSample = 0;
}

void manta::Sampler::endSample() {
    m_currentPixelSample++;
}
