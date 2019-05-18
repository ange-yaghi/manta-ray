#include <lens_camera_ray_emitter.h>

#include <light_ray.h>
#include <lens.h>
#include <sampler_2d.h>
#include <stack_allocator.h>
#include <ray_container.h>

manta::LensCameraRayEmitter::LensCameraRayEmitter() {
	m_lens = nullptr;
}

manta::LensCameraRayEmitter::~LensCameraRayEmitter() {
	/* void */
}

void manta::LensCameraRayEmitter::generateRays(RayContainer *rayContainer) const {
	int totalRayCount = m_sampler->getTotalSampleCount(m_sampleCount);
	
	// Create all rays
	rayContainer->initializeRays(totalRayCount);
	rayContainer->setDegree(0);
	LightRay *rays = rayContainer->getRays();

	math::Vector *sampleOrigins = 
		(math::Vector *)m_stackAllocator->allocate(sizeof(math::Vector) * totalRayCount, 16);
	m_sampler->generateSamples(totalRayCount, sampleOrigins);

	Lens::LensScanHint hint;
	m_lens->lensScan(m_position, 4, m_sampler->getBoundaryWidth(), &hint);

	for (int i = 0; i < totalRayCount; i++) {
		math::Vector position = math::add(m_position, sampleOrigins[i]);

		rays[i].setIntensity(math::constants::Zero);
		rays[i].setWeight(math::constants::One);

		bool result = m_lens->generateOutgoingRay(position, &hint, &rays[i]);
	}

	m_stackAllocator->free((void *)sampleOrigins);
}
