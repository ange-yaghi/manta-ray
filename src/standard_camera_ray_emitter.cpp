#include <standard_camera_ray_emitter.h>

#include <sampler_2d.h>
#include <light_ray.h>
#include <stack_allocator.h>

manta::StandardCameraRayEmitter::StandardCameraRayEmitter() {
}

manta::StandardCameraRayEmitter::~StandardCameraRayEmitter() {
}

void manta::StandardCameraRayEmitter::generateRays() {
	int nRays = m_sampler->getTotalSampleCount(m_sampleCount);
	
	// InitializeRays
	initializeRays(nRays);
	LightRay *rays = getRays();
	
	math::Vector *samplePoints = (math::Vector *)getStackAllocator()->allocate(nRays * sizeof(math::Vector), 16);
	m_sampler->generateSamples(nRays, samplePoints);

	for (int i = 0; i < nRays; i++) {
		LightRay *ray = &rays[i];
		math::Vector target = math::add(m_targetOrigin, samplePoints[i]);
		math::Vector dir = math::sub(target, m_position);
		dir = math::normalize(dir);

		ray->setDirection(dir);
		ray->setSource(m_position);
		ray->setIntensity(math::constants::Zero);
		ray->setWeight((math::real)1.0);
	}

	getStackAllocator()->free((void *)samplePoints);
}
