#include <camera_ray_emitter.h>

#include <light_ray.h>

manta::CameraRayEmitter::CameraRayEmitter() {
	m_sampler = nullptr;

	m_position = math::constants::Zero;
	m_direction = math::constants::Zero;
	m_sampleCount = 0;
}

manta::CameraRayEmitter::~CameraRayEmitter() {
}
