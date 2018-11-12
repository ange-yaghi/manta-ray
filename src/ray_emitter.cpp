#include <ray_emitter.h>
#include <light_ray.h>

#include <malloc.h>
#include <new>

manta::RayEmitter::RayEmitter() {
	m_rayCount = 0;
	m_rays = nullptr;
	m_samplesPerRay = 1;
}

manta::RayEmitter::~RayEmitter() {
	destroyRays();
}

void manta::RayEmitter::initializeRays(int count) {
	size_t size = sizeof(LightRay) * count;
	void *buffer = _aligned_malloc(size, 16);

	m_rays = (LightRay *)buffer;
	m_rayCount = count;

	for (int i = 0; i < m_rayCount; i++) {
		m_rays[i].setIntensity(math::constants::Zero);
	}
}

void manta::RayEmitter::destroyRays() {
	if (m_rays == nullptr) return;

	_aligned_free((void *)m_rays);
	m_rays = nullptr;
}
