#include "../include/pixel_based_sampler.h"

manta::PixelBasedSampler::PixelBasedSampler() {
    m_current1dDimension = m_current2dDimension = 0;
}

manta::PixelBasedSampler::~PixelBasedSampler() {
    /* void */
}

void manta::PixelBasedSampler::initialize(int samplesPerPixel, int dimensionCount) {
    for (int i = 0; i < dimensionCount; i++) {
        m_1dSamples.push_back(std::vector<math::real>(samplesPerPixel));
        m_2dSamples.push_back(std::vector<math::Vector2>(samplesPerPixel));
    }
}

manta::math::real manta::PixelBasedSampler::generate1d() {
    if (m_current1dDimension < m_1dSamples.size()) {
        return m_1dSamples[m_current1dDimension++][m_currentPixelSample];
    }
    else return math::uniformRandom();
}

manta::math::Vector2 manta::PixelBasedSampler::generate2d() {
    if (m_current1dDimension < m_1dSamples.size()) {
        return m_2dSamples[m_current1dDimension++][m_currentPixelSample];
    }
    else {
        return math::Vector2(
            math::uniformRandom(),
            math::uniformRandom());
    }
}

void manta::PixelBasedSampler::endSample() {
    Sampler::endSample();
    m_current1dDimension = m_current2dDimension = 0;
}
