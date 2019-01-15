#include <lens_camera_ray_emitter.h>

#include <light_ray.h>
#include <lens.h>

manta::LensCameraRayEmitter::LensCameraRayEmitter() {
	m_explicitSampleCount = 0;
}

manta::LensCameraRayEmitter::~LensCameraRayEmitter() {

}

void manta::LensCameraRayEmitter::generateRays() {
	// Create all rays
	initializeRays(m_explicitSampleCount);
	LightRay *rays = getRays();

	LensScanHint hint;
	m_lens->lensScan(m_position, &hint, 4);

	for (int i = 0; i < m_explicitSampleCount; i++) {
		bool result = m_lens->generateOutgoingRay(m_position, &hint, &rays[i]);
		rays[i].setIntensity(math::constants::Zero);
	}
}

void manta::LensCameraRayEmitter::calculateIntensity() {
	LightRay *rays = getRays();
	int rayCount = getRayCount();

	math::Vector accum = math::constants::Zero;
	for (int i = 0; i < rayCount; i++) {
		math::Vector clamped = rays[i].getWeightedIntensity();

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
	m_intensity = accum;
}
