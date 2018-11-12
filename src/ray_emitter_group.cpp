#include <ray_emitter_group.h>

#include <ray_emitter.h>

manta::RayEmitterGroup::RayEmitterGroup() {
	m_degree = 0;
}

manta::RayEmitterGroup::~RayEmitterGroup() {
	destroyEmitters();
}

void manta::RayEmitterGroup::destroyEmitters() {
	for (int i = 0; i < m_rayEmitterCount; i++) {
		m_rayEmitters[i]->~RayEmitter();
		_aligned_free((void *)m_rayEmitters[i]);
	}

	delete[] m_rayEmitters;

	m_rayEmitters = nullptr;
	m_rayEmitterCount = 0;
}

void manta::RayEmitterGroup::initializeEmitters(int count) {
	m_rayEmitterCount = count;
	m_rayEmitters = new RayEmitter *[count];
}
