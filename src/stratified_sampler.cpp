#include "../include/stratified_sampler.h"

#include <algorithm>

manta::StratifiedSampler::StratifiedSampler() {
    m_latticeWidth = m_latticeHeight = 4;
    m_jitter = true;
}

manta::StratifiedSampler::~StratifiedSampler() {
    /* void */
}

void manta::StratifiedSampler::startPixelSession() {
    PixelBasedSampler::startPixelSession();

    for (size_t i = 0; i < m_1dSamples.size(); i++) {
        stratifiedSample1d(m_1dSamples[i], m_latticeWidth * m_latticeHeight, m_jitter);
        shuffle(m_1dSamples[i], m_latticeWidth * m_latticeHeight, 1);
    }

    for (size_t i = 0; i < m_2dSamples.size(); i++) {
        stratifiedSample2d(m_2dSamples[i], m_latticeWidth, m_latticeHeight, m_jitter);
        shuffle(m_2dSamples[i], m_latticeWidth * m_latticeHeight, 1);
    } 
}

void manta::StratifiedSampler::stratifiedSample1d(std::vector<math::real> &samples, 
    int sampleCount, bool jitter) 
{
    math::real invSampleCount = (math::real)1.0 / sampleCount;
    for (int i = 0; i < sampleCount; i++) {
        math::real delta = jitter 
            ? math::uniformRandom() 
            : (math::real)0.5f;
        samples[i] = std::min((i + delta) * invSampleCount, 0.9999f);
    }
}

void manta::StratifiedSampler::stratifiedSample2d(std::vector<math::Vector2> &samples, 
    int latticeWidth, int latticeHeight, bool jitter) 
{
    math::real dx = (math::real)1.0 / latticeWidth;
    math::real dy = (math::real)1.0 / latticeHeight;

    for (int y = 0; y < latticeHeight; y++) {
        for (int x = 0; x < latticeWidth; x++) {
            math::real delta_x = jitter
                ? math::uniformRandom()
                : (math::real)0.5f;
            math::real delta_y = jitter
                ? math::uniformRandom()
                : (math::real)0.5f;

            math::Vector2 &s = samples[y * latticeWidth + x];
            s.x = std::min((x + delta_x) * dx, 0.9999f);
            s.y = std::min((y + delta_y) * dy, 0.9999f);
        }
    }
}
