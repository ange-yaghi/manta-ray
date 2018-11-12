#include <simple_ray_emitter_group.h>

manta::SimpleRayEmitterGroup::SimpleRayEmitterGroup() {
	m_simpleRayEmitter = nullptr;
}

manta::SimpleRayEmitterGroup::~SimpleRayEmitterGroup() {

}

void manta::SimpleRayEmitterGroup::createAllEmitters() {
	initializeEmitters(1);
	m_simpleRayEmitter = createEmitter<SimpleRayEmitter>();

	m_rayEmitters[0] = (RayEmitter *)m_simpleRayEmitter;
}
