#include <simple_scatter_emitter_group.h>

#include <simple_scatter_emitter.h>

manta::SimpleScatterEmitterGroup::SimpleScatterEmitterGroup() {
	m_simpleRayEmitter = nullptr;
}

manta::SimpleScatterEmitterGroup::~SimpleScatterEmitterGroup() {

}

void manta::SimpleScatterEmitterGroup::createAllEmitters() {
	initializeEmitters(1);
	m_simpleRayEmitter = createEmitter<SimpleScatterEmitter>();

	m_rayEmitters[0] = (RayEmitter *)m_simpleRayEmitter;
}
