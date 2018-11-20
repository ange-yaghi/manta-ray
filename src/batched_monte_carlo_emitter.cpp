#include <batched_monte_carlo_emitter.h>

#include <light_ray.h>

manta::BatchedMonteCarloEmitter::BatchedMonteCarloEmitter() {
}

manta::BatchedMonteCarloEmitter::~BatchedMonteCarloEmitter() {
}

void manta::BatchedMonteCarloEmitter::generateRays() {
	initializeRays(m_samples);

	math::Vector realNormal = math::dot(m_incidentDirection, m_normal);

	if (math::getScalar(realNormal) < 0.0f) {
		realNormal = m_normal;
	}
	else {
		realNormal = math::negate(m_normal);
	}

	math::Vector u = math::constants::YAxis;
	math::Vector v;
	math::Vector w = realNormal;
	if (abs(math::getX(realNormal)) < 0.1f) {
		u = math::constants::XAxis;
	}
	u = math::normalize(math::cross(u, realNormal));
	v = math::cross(realNormal, u);

	for (int i = 0; i < m_samples; i++) {
		math::real r1 = math::uniformRandom(math::constants::TWO_PI);
		math::real r2 = math::uniformRandom();
		math::real r2s = (math::real)sqrt(r2);

		math::Vector d = math::add(
			math::mul(u, math::loadScalar(cos(r1) * r2s)),
			math::mul(v, math::loadScalar(sin(r1) * r2s)));
		d = math::add(
			d,
			math::mul(w, math::loadScalar(sqrt(1 - r2))));
		d = math::normalize(d);

		LightRay &ray = getRays()[i];
		ray.setDirection(d);
		ray.setSource(getPosition());
	}
}

manta::math::Vector manta::BatchedMonteCarloEmitter::getAverageIntensity() const {
	math::Vector accum = math::constants::Zero;
	for (int i = 0; i < m_samples; i++) {
		accum = math::add(accum, getRays()[i].getIntensity());
	}

	return math::div(accum, math::loadScalar((math::real)m_samples));
}
