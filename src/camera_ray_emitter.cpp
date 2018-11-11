#include <camera_ray_emitter.h>

#include <light_ray.h>

manta::CameraRayEmitter::CameraRayEmitter() {

}

manta::CameraRayEmitter::~CameraRayEmitter() {

}

void manta::CameraRayEmitter::generateRays() {
	/*
	int nRays = m_resolutionX * m_resolutionY;
	float planeWidth = (m_resolutionX / (float)m_resolutionY) * m_planeHeight;

	float xIncrement = planeWidth / m_resolutionX;
	float yIncrement = m_planeHeight / m_resolutionY;

	initializeRays(nRays);
	//initializeRays(1);

	LightRay *rays = getRays();

	math::Vector right = math::cross(m_direction, m_up);

	math::Vector planeCenter = math::add(m_position, math::mul(m_direction, math::loadScalar(m_planeDistance)));
	// 1015, 1042
	for (int i = 0; i < m_resolutionY; i++) {
		for (int j = 0; j < m_resolutionX; j++) {
			//if (j != 966 || i != 1324) continue;

			float x = xIncrement * j;
			float y = yIncrement * i;

			math::Vector loc = math::mul(right, math::loadScalar(x - planeWidth / 2.0f));
			loc = math::sub(loc, math::mul(m_up, math::loadScalar(y - m_planeHeight / 2.0f)));
			loc = math::add(loc, planeCenter);

			//LightRay *ray = &rays[0];  
			LightRay *ray = &rays[i * m_resolutionX + j];
			math::Vector direction = math::sub(loc, m_position);
			direction = math::normalize(direction);

			ray->setDirection(direction);
			ray->setSource(m_position);
		}
	}
	*/

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

manta::math::Vector manta::CameraRayEmitter::getIntensity() const {
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
