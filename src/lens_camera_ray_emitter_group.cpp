#include <lens_camera_ray_emitter_group.h>

#include <lens_camera_ray_emitter.h>
#include <lens.h>
#include <sampler_2d.h>

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

	math::real sensorElementWidth = m_lens->getSensorWidth() / getResolutionX();
	math::real sensorElementHeight = m_lens->getSensorHeight() / getResolutionY();

	m_sampler->setBoundaryWidth(sensorElementWidth);
	m_sampler->setBoundaryHeight(sensorElementHeight);
	m_sampler->setAxis1(m_lens->getSensorRight());
	m_sampler->setAxis2(m_lens->getSensorUp());

	initializeEmitters(nRays);

	for (int i = 0; i < resolutionY; i++) {
		for (int j = 0; j < resolutionX; j++) {
			LensCameraRayEmitter *newEmitter = createEmitter<LensCameraRayEmitter>();
			m_rayEmitters[i * resolutionX + j] = (RayEmitter *)newEmitter;

			newEmitter->setLens(m_lens);

			math::Vector sensorElement = m_lens->getSensorElement(j, i);
			newEmitter->setSampleCount(m_samples);
			newEmitter->setSamplesPerRay(1);
			newEmitter->setPosition(sensorElement);
			newEmitter->setSampler(m_sampler);
		}
	}
}
