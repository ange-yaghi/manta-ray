#include <phong_emitter_group.h>

#include <phong_emitter.h>
#include <hemi_monte_carlo_emitter.h>

manta::PhongEmitterGroup::PhongEmitterGroup() {
}

manta::PhongEmitterGroup::~PhongEmitterGroup() {
}

void manta::PhongEmitterGroup::createAllEmitters() {
	initializeEmitters(1);
	m_rayEmitters[0] = createEmitter<PhongEmitter>();
}
