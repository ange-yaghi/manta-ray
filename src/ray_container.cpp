#include <ray_container.h>

#include <light_ray.h>
#include <stack_allocator.h>

#include <assert.h>

manta::RayContainer::RayContainer() {
	m_degree = 0;
	m_intensity = math::constants::Zero;
	m_rayCount = 0;
	m_rays = nullptr;
	m_stackAllocator = nullptr;
	m_samplesPerRay = 0;
}

manta::RayContainer::~RayContainer() {
	assert(m_rays == nullptr);
}

void manta::RayContainer::initializeRays(int count) {
	size_t size = sizeof(LightRay) * count;
	void *buffer = nullptr;

	if (m_stackAllocator == nullptr) {
		buffer = _aligned_malloc(size, 16);
	}
	else {
		buffer = m_stackAllocator->allocate((unsigned int)size, 16);
	}

	m_rays = (LightRay *)buffer;
	m_rayCount = count;

	for (int i = 0; i < m_rayCount; i++) {
		m_rays[i].setIntensity(math::constants::Zero);
		m_rays[i].setWeight((math::real)1.0);
	}
}

void manta::RayContainer::destroyRays() {
	if (m_rays == nullptr) return;

	if (m_stackAllocator == nullptr) {
		_aligned_free((void *)m_rays);
	}
	else {
		m_stackAllocator->free((void *)m_rays);
	}

	m_rays = nullptr;
}

void manta::RayContainer::calculateIntensity() {
	math::Vector sum = math::constants::Zero;
	for (int i = 0; i < m_rayCount; i++) {
		sum = math::add(sum, m_rays[i].getWeightedIntensity());
	}
	m_intensity = math::div(sum, math::loadScalar((math::real)m_rayCount));
}
