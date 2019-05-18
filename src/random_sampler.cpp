#include <random_sampler.h>

manta::RandomSampler::RandomSampler() {
	/* void */
}

manta::RandomSampler::~RandomSampler() {
	/* void */
}

void manta::RandomSampler::generateSamples(int sampleCount, math::Vector *target) const {
	for (int s = 0; s < sampleCount; s++) {
		math::real r1 = math::uniformRandom();
		math::real r2 = math::uniformRandom();

		math::real x = (r1 - (math::real)0.5) * m_boundaryWidth;
		math::real y = (r2 - (math::real)0.5) * m_boundaryHeight;

		math::Vector u = math::mul(m_axis1, math::loadScalar(x));
		math::Vector v = math::mul(m_axis2, math::loadScalar(y));
		math::Vector rayTarget = math::add(u, v);

		target[s] = rayTarget;
	}
}

int manta::RandomSampler::getTotalSampleCount(int sampleCount) const {
	return sampleCount;
}
