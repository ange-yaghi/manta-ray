#include <camera_ray_emitter.h>

#include <light_ray.h>

manta::CameraRayEmitter::CameraRayEmitter() {
	m_sampler = nullptr;
}

manta::CameraRayEmitter::~CameraRayEmitter() {
}

void manta::CameraRayEmitter::calculateIntensity() {
	LightRay *rays = getRays();
	int rayCount = getRayCount();

	if (rayCount == 0) {
		m_intensity = math::constants::Zero;
		return;
	}

	math::Vector accum = math::constants::Zero;
	for (int i = 0; i < rayCount; i++) {
		math::Vector rayIntensity = rays[i].getWeightedIntensity();
		accum = math::add(accum, rayIntensity);
	}

	accum = math::div(accum, math::loadScalar((math::real)rayCount));
	m_intensity = accum;
}
