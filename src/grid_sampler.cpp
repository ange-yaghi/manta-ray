#include <grid_sampler.h>

#include <assert.h>

manta::GridSampler::GridSampler() {
	m_gridWidth = 0;
}

manta::GridSampler::~GridSampler() {
	/* void */
}

void manta::GridSampler::generateSamples(int sampleCount, math::Vector *target) const {
	assert(m_gridWidth > 0);

	int latticePoints = m_gridWidth * m_gridWidth;
	int actualSampleCount = getTotalSampleCount(sampleCount);
	actualSampleCount /= latticePoints;

	math::real xStep = m_boundaryWidth / m_gridWidth;
	math::real yStep = m_boundaryHeight / m_gridWidth;

	// Find the origin
	math::real offset;
	if (m_gridWidth % 2 == 0) {
		// Even sampling
		offset = (math::real)0.5;
	}
	else {
		offset = (math::real)0.0;
	}

	int half = m_gridWidth / 2;

	offset = -half + offset;

	for (int s = 0; s < actualSampleCount; s++) {
		for (int i = 0; i < m_gridWidth; i++) {
			for (int j = 0; j < m_gridWidth; j++) {
				math::real x = (j + offset) * xStep;
				math::real y = (i + offset) * yStep;

				math::Vector u = math::mul(m_axis1, math::loadScalar(x));
				math::Vector v = math::mul(m_axis2, math::loadScalar(y));
				math::Vector rayTarget = math::add(u, v);

				target[s * latticePoints + i * m_gridWidth + j] = rayTarget;
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
