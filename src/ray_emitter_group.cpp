#include <ray_emitter_group.h>

#include <ray_emitter.h>

manta::RayEmitterGroup::RayEmitterGroup() {
	m_degree = 0;
	m_stackAllocator = nullptr;
}

manta::RayEmitterGroup::~RayEmitterGroup() {
	assert(m_rayEmitters == nullptr);
}

void manta::RayEmitterGroup::destroyEmitters() {
	if (m_rayEmitterCount == 0) return;

	// Make sure to destroy the emitters in reverse
	for (int i = m_rayEmitterCount - 1; i >= 0; i--) {
		m_rayEmitters[i]->~RayEmitter();

		if (m_stackAllocator == nullptr) {
			_aligned_free((void *)m_rayEmitters[i]);
		}
		else {
			m_stackAllocator->free((void *)m_rayEmitters[i]);
		}
	}

	if (m_stackAllocator == nullptr) {
		delete[] m_rayEmitters;
	} 
	else {
		m_stackAllocator->free((void *)m_rayEmitters);
	}

	m_rayEmitters = nullptr;
	m_rayEmitterCount = 0;
}

void manta::RayEmitterGroup::initializeEmitters(int count) {
	m_rayEmitterCount = count;

	if (count > 0) {
		if (m_stackAllocator == nullptr) {
			m_rayEmitters = new RayEmitter *[count];
		}
		else {
			m_rayEmitters = (RayEmitter **)m_stackAllocator->allocate(sizeof(RayEmitter *) * count);
		}
	}
	else {
		m_rayEmitters = nullptr;
	}
}
