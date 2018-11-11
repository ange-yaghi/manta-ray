#include <simple_monte_carlo_emitter.h>

#include <light_ray.h>
#include <manta_math.h>

#define _USE_MATH_DEFINES
#include <random>
#include <math.h>

manta::SimpleMonteCarloEmitter::SimpleMonteCarloEmitter() {

}

manta::SimpleMonteCarloEmitter::~SimpleMonteCarloEmitter() {

}

void manta::SimpleMonteCarloEmitter::generateRays() {
	initializeRays(1);

	math::real r1 = math::uniformRandom(2.0 * M_PI);
	math::real r2 = math::uniformRandom();
	math::real r2s = (math::real)sqrt(r2);

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
	if (math::getX(realNormal) < 0.1f) {
		u = math::constants::XAxis;
	}
	u = math::normalize(math::cross(u, realNormal));
	v = math::cross(realNormal, u);

	math::Vector d = math::add(
		math::mul(u, math::loadScalar(cos(r1) * r2s)),
		math::mul(v, math::loadScalar(sin(r1) * r2s)));
	d = math::add(
		d,
		math::mul(w, math::loadScalar(sqrt(1 - r2))));
	d = math::normalize(d);

	LightRay *mainRay = getRays();
	mainRay->setDirection(d);
	mainRay->setSource(getPosition());

	m_singleRay = mainRay;
}
