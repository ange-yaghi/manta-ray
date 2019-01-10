#include <batched_monte_carlo_emitter_group.h>

manta::BatchedMonteCarloEmitterGroup::BatchedMonteCarloEmitterGroup() {
	m_simpleRayEmitter = nullptr;
	m_texCoord = math::constants::Zero;
}

manta::BatchedMonteCarloEmitterGroup::~BatchedMonteCarloEmitterGroup() {

}

void manta::BatchedMonteCarloEmitterGroup::createAllEmitters() {
	initializeEmitters(1);
	m_simpleRayEmitter = createEmitter<BatchedMonteCarloEmitter>();

	m_rayEmitters[0] = (RayEmitter *)m_simpleRayEmitter;
}
