#include <simple_scatter_emitter.h>

#include <light_ray.h>
#include <manta_math.h>

#include <random>
#include <math.h>

#include <iostream>

manta::SimpleScatterEmitter::SimpleScatterEmitter() {
	m_singleRay = nullptr;
}

manta::SimpleScatterEmitter::~SimpleScatterEmitter() {
}

void manta::SimpleScatterEmitter::generateRays() {
	initializeRays(1);

	math::real r1 = math::uniformRandom(math::constants::TWO_PI);
	math::real r2 = math::uniformRandom((math::real)2.0) - (math::real)1.0;

	math::Vector realNormal = m_direction;

	if (r2 < (math::real)0.0) {
		realNormal = math::negate(realNormal);
		r2 = -r2;
	}

	math::real r2s = (math::real)sqrt(r2);

	math::Vector u = math::constants::YAxis;
	math::Vector v;
	math::Vector w = realNormal;
	if (abs(math::getX(realNormal)) < 0.1f) {
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
