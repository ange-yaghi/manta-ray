#include "monte_carlo_specular_diffuse_group.h"

manta::MonteCarloSpecularDiffuseGroup::MonteCarloSpecularDiffuseGroup() {
	m_texCoord = math::constants::Zero;
}

manta::MonteCarloSpecularDiffuseGroup::~MonteCarloSpecularDiffuseGroup() {
}

void manta::MonteCarloSpecularDiffuseGroup::createAllEmitters() {
	int emitterCount = 0;
	if (m_diffuseEnabled) emitterCount++;
	if (m_specularEnabled) emitterCount++;

	initializeEmitters(emitterCount);
	if (m_diffuseEnabled) m_diffuseEmitter = createEmitter<BatchedMonteCarloEmitter>();
	else m_diffuseEmitter = nullptr;

	if (m_specularEnabled) m_specularEmitter = createEmitter<SimpleRayEmitter>();
	else m_specularEmitter = nullptr;

	if (emitterCount > 0) {
		// TODO: remove this hack
		m_rayEmitters[0] = (RayEmitter *)m_diffuseEmitter;
		if (m_specularEnabled && m_diffuseEnabled) m_rayEmitters[1] = (RayEmitter *)m_specularEmitter;
		else if (m_specularEnabled) m_rayEmitters[0] = (RayEmitter *)m_specularEmitter;
	}
}
