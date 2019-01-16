#include <hemi_monte_carlo_emitter.h>

#include <light_ray.h>

manta::HemiMonteCarloEmitter::HemiMonteCarloEmitter() {
	m_samples = 1;
}

manta::HemiMonteCarloEmitter::~HemiMonteCarloEmitter() {
}

void manta::HemiMonteCarloEmitter::generateRays() {
	initializeRays(m_samples);

	math::Vector u = math::constants::YAxis;
	math::Vector v;
	math::Vector w = m_normal;
	if (abs(math::getX(m_normal)) < 0.1f) {
		u = math::constants::XAxis;
	}
	u = math::normalize(math::cross(u, m_normal));
	v = math::cross(m_normal, u);

	for (int i = 0; i < m_samples; i++) {
		math::real r1 = math::uniformRandom(math::constants::TWO_PI);
		math::real r2 = math::uniformRandom();
		math::real r2s = (math::real)sqrt(r2);

		math::Vector direction = math::add(
			math::mul(u, math::loadScalar(cos(r1) * r2s)),
			math::mul(v, math::loadScalar(sin(r1) * r2s)));
		direction = math::add(
			direction,
			math::mul(w, math::loadScalar(sqrt(1 - r2))));
		direction = math::normalize(direction);

		LightRay &ray = getRays()[i];
		ray.setDirection(direction);
		ray.setSource(getPosition());
		ray.setIntensity(math::constants::Zero);
	}
}
