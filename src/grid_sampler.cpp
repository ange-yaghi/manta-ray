#include "../include/grid_sampler.h"

#include <assert.h>

manta::GridSampler::GridSampler() {
    m_gridWidth = 0;
    m_gridWidthInput = nullptr;
}

manta::GridSampler::~GridSampler() {
    /* void */
}

void manta::GridSampler::generateSamples(int sampleCount, math::Vector2 *target) const {
    assert(m_gridWidth > 0);

    int latticePoints = m_gridWidth * m_gridWidth;
    int actualSampleCount = getTotalSampleCount(sampleCount);
    actualSampleCount /= latticePoints;

    math::real xStep = (math::real)1.0 / m_gridWidth;
    math::real yStep = (math::real)1.0 / m_gridWidth;

    for (int s = 0; s < actualSampleCount; s++) {
        for (int i = 0; i < m_gridWidth; i++) {
            for (int j = 0; j < m_gridWidth; j++) {
                math::real x = (j + (math::real)0.5) * xStep;
                math::real y = (i + (math::real)0.5) * yStep;

                target[s * latticePoints + i * m_gridWidth + j] = math::Vector2(x, y);
            }
        }
    }
}

int manta::GridSampler::getTotalSampleCount(int sampleCount) const {
    int actualSampleCount = sampleCount;
    int latticePoints = m_gridWidth * m_gridWidth;

    // Resolve the actual sample count
    int s_mod_l = actualSampleCount % latticePoints;
    if (s_mod_l != 0) {
        actualSampleCount += (latticePoints - s_mod_l);
    }

    return actualSampleCount;
}

void manta::GridSampler::_initialize() {
    /* void */
}

void manta::GridSampler::_evaluate() {
    static_cast<piranha::IntValueOutput *>(m_gridWidthInput)->fullCompute((void *)&m_gridWidth);
}

void manta::GridSampler::registerInputs() {
    registerInput(&m_gridWidthInput, "grid_width");
}
