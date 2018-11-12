#include <simple_monte_carlo_emitter_group.h>

manta::SimpleMonteCarloEmitterGroup::SimpleMonteCarloEmitterGroup() {
	m_simpleRayEmitter = nullptr;
}

manta::SimpleMonteCarloEmitterGroup::~SimpleMonteCarloEmitterGroup() {

}

void manta::SimpleMonteCarloEmitterGroup::createAllEmitters() {
	initializeEmitters(1);
	m_simpleRayEmitter = createEmitter<SimpleMonteCarloEmitter>();

	m_rayEmitters[0] = (RayEmitter *)m_simpleRayEmitter;
}
