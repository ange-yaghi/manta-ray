#include <camera_ray_emitter_group.h>

#include <scene_buffer.h>
#include <ray_emitter.h>

manta::CameraRayEmitterGroup::CameraRayEmitterGroup() {
}

manta::CameraRayEmitterGroup::~CameraRayEmitterGroup() {
}

void manta::CameraRayEmitterGroup::destroyRays() {
	// Destroy the rays in reverse order
	for (int i = getEmitterCount() - 1; i >= 0; i--) {
		getEmitters()[i]->destroyRays();
	}
}

void manta::CameraRayEmitterGroup::fillSceneBuffer(SceneBuffer *sceneBuffer) const {
	sceneBuffer->initialize(m_resolutionX, m_resolutionY);

	for (int x = 0; x < m_resolutionX; x++) {
		for (int y = 0; y < m_resolutionY; y++) {
			sceneBuffer->set(getEmitters()[y * m_resolutionX + x]->getIntensity(), x, y);
		}
	}
}
