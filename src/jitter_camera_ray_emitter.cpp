#include <jitter_camera_ray_emitter.h>

#include <light_ray.h>

manta::JitterCameraRayEmitter::JitterCameraRayEmitter() {
	m_explicitSampleCount = 1;
	m_width = (math::real)0.0;
	m_height = (math::real)0.0;
}

manta::JitterCameraRayEmitter::~JitterCameraRayEmitter() {

}

void manta::JitterCameraRayEmitter::generateRays() {
	// Create all rays
	initializeRays(m_explicitSampleCount);
	LightRay *rays = getRays();

	for (int i = 0; i < m_explicitSampleCount; i++) {
		math::real jitterX = math::uniformRandom(m_width) - (m_width / (math::real)2.0);
		math::real jitterY = math::uniformRandom(m_height) - (m_height / (math::real)2.0);

		math::Vector u = math::mul(m_axisX, math::loadScalar(jitterX));
		math::Vector v = math::mul(m_axisY, math::loadScalar(jitterY));
		math::Vector target = math::add(u, v);
		target = math::add(
			target,
			m_startPosition
		);
		math::Vector dir = math::sub(target, m_position);
		dir = math::normalize(dir);

		LightRay *ray = &rays[i];
		ray->setDirection(dir);
		ray->setSource(m_position);
		ray->setIntensity(math::constants::Zero);
		ray->setWeight((math::real)1.0);
	}
}

void manta::JitterCameraRayEmitter::calculateIntensity() {
	LightRay *rays = getRays();
	int rayCount = getRayCount();

	if (rayCount == 0) {
		m_intensity = math::constants::Zero;
		return;
	}

	math::Vector accum = math::constants::Zero;
	for (int i = 0; i < rayCount; i++) {
		math::Vector clamped = rays[i].getWeightedIntensity();
		accum = math::add(accum, clamped);
	}

	accum = math::div(accum, math::loadScalar((math::real)rayCount));

	math::real r = math::getX(accum);
	math::real g = math::getY(accum);
	math::real b = math::getZ(accum);

	r = math::clamp(r);
	g = math::clamp(g);
	b = math::clamp(b);

	m_intensity = math::loadVector(r, g, b);
}
