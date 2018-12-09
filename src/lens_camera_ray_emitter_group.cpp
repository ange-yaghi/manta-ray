#include <lens_camera_ray_emitter_group.h>

#include <lens_camera_ray_emitter.h>
#include <lens.h>

manta::LensCameraRayEmitterGroup::LensCameraRayEmitterGroup() {
	m_lens = nullptr;
}

manta::LensCameraRayEmitterGroup::~LensCameraRayEmitterGroup() {

}

void manta::LensCameraRayEmitterGroup::createAllEmitters() {
	assert(m_lens != nullptr);

	int resolutionX = m_lens->getSensorResolutionX();
	int resolutionY = m_lens->getSensorResolutionY();

	int nRays = resolutionX * resolutionY;

	initializeEmitters(nRays);

	for (int i = 0; i < resolutionY; i++) {
		for (int j = 0; j < resolutionX; j++) {
			LensCameraRayEmitter *newEmitter = createEmitter<LensCameraRayEmitter>();
			m_rayEmitters[i * resolutionX + j] = (RayEmitter *)newEmitter;

			newEmitter->setLens(m_lens);

			math::Vector sensorElement = m_lens->getSensorElement(j, i);
			newEmitter->setSamplesPerRay(m_samplesPerPixel);
			newEmitter->setPosition(sensorElement);
			newEmitter->setExplicitSampleCount(m_explicitSampleCount);
		}
	}
}
