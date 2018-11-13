#include <ray_emitter.h>
#include <light_ray.h>

#include <stack_allocator.h>

#include <assert.h>
#include <malloc.h>
#include <new>

manta::RayEmitter::RayEmitter() {
	m_rayCount = 0;
	m_rays = nullptr;
	m_samplesPerRay = 1;

	m_stackAllocator = nullptr;
}

manta::RayEmitter::~RayEmitter() {
	assert(m_rays == nullptr);
}

void manta::RayEmitter::initializeRays(int count) {
	size_t size = sizeof(LightRay) * count;
	void *buffer = nullptr;

	if (m_stackAllocator == nullptr) {
		buffer = _aligned_malloc(size, 16);
	}
	else {
		buffer = m_stackAllocator->allocate(size, 16);
	}

	m_rays = (LightRay *)buffer;
	m_rayCount = count;

	for (int i = 0; i < m_rayCount; i++) {
		m_rays[i].setIntensity(math::constants::Zero);
	}
}

void manta::RayEmitter::destroyRays() {
	if (m_rays == nullptr) return;

	if (m_stackAllocator == nullptr) {
		_aligned_free((void *)m_rays);
	}
	else {
		m_stackAllocator->free((void *)m_rays);
	}

	m_rays = nullptr;
}
