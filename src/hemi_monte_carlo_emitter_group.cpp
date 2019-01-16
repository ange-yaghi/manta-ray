#include "hemi_monte_carlo_emitter_group.h"

#include <hemi_monte_carlo_emitter.h>

manta::HemiMonteCarloEmitterGroup::HemiMonteCarloEmitterGroup() {
	m_texCoord = math::constants::Zero;
}

manta::HemiMonteCarloEmitterGroup::~HemiMonteCarloEmitterGroup() {
}

void manta::HemiMonteCarloEmitterGroup::createAllEmitters() {
	initializeEmitters(m_layerCount);
	for (int i = 0; i < m_layerCount; i++) {
		m_rayEmitters[i] = createEmitter<HemiMonteCarloEmitter>();
	}
}
