#include <lens_camera_ray_emitter.h>

#include <light_ray.h>

manta::LensCameraRayEmitter::LensCameraRayEmitter() {

}

manta::LensCameraRayEmitter::~LensCameraRayEmitter() {

}

void manta::LensCameraRayEmitter::generateRays() {
	// Create all rays
	initializeRays(m_samples * m_samples);
	LightRay *rays = getRays();

	// Find the origin
	math::real offset;
	if (m_samples % 2 == 0) {
		// Even sampling
		offset = (math::real)0.5;
	}
	else {
		offset = (math::real)0.0;
	}

	int half = m_samples / 2;

	offset = -half + offset;

	for (int i = 0; i < m_samples; i++) {
		for (int j = 0; j < m_samples; j++) {
			math::real x = i + offset;
			math::real y = j + offset;

			math::Vector u = math::mul(m_sampleOffsetX, math::loadScalar(x));
			math::Vector v = math::mul(m_sampleOffsetY, math::loadScalar(y));
			math::Vector dir = math::add(u, v);
			dir = math::add(
				dir,
				m_startDirection
			);
			dir = math::normalize(dir);

			LightRay *ray = &rays[i * m_samples + j];
			ray->setDirection(dir);
			ray->setSource(m_position);
		}
	}
}

manta::math::Vector manta::LensCameraRayEmitter::getIntensity() const {
	LightRay *rays = getRays();
	int rayCount = getRayCount();

	math::Vector accum = math::constants::Zero;
	for (int i = 0; i < rayCount; i++) {
		math::Vector clamped = rays[i].getIntensity();

		math::real r = math::getX(clamped);
		math::real g = math::getY(clamped);
		math::real b = math::getZ(clamped);

		r = math::clamp(r);
		g = math::clamp(g);
		b = math::clamp(b);

		clamped = math::loadVector(r, g, b);

		accum = math::add(accum, clamped);
	}

	accum = math::div(accum, math::loadScalar((math::real)rayCount));
	return accum;
}
