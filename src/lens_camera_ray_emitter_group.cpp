#include <lens_camera_ray_emitter_group.h>

#include <lens_camera_ray_emitter.h>
#include <lens.h>
#include <sampler_2d.h>

manta::LensCameraRayEmitterGroup::LensCameraRayEmitterGroup() {
	m_lens = nullptr;
}

manta::LensCameraRayEmitterGroup::~LensCameraRayEmitterGroup() {
	/* void */
}

void manta::LensCameraRayEmitterGroup::initialize() {
	int resolutionX = m_lens->getSensorResolutionX();
	int resolutionY = m_lens->getSensorResolutionY();

	int nRays = resolutionX * resolutionY;

	math::real sensorElementWidth = m_lens->getSensorWidth() / getResolutionX();
	math::real sensorElementHeight = m_lens->getSensorHeight() / getResolutionY();

	m_sampler->setBoundaryWidth(sensorElementWidth);
	m_sampler->setBoundaryHeight(sensorElementHeight);
	m_sampler->setAxis1(m_lens->getSensorRight());
	m_sampler->setAxis2(m_lens->getSensorUp());
}

manta::CameraRayEmitter *manta::LensCameraRayEmitterGroup::createEmitter(int ix, int iy, 
	StackAllocator *stackAllocator) const {
	LensCameraRayEmitter *newEmitter = allocateEmitter<LensCameraRayEmitter>(stackAllocator);

	math::Vector sensorElement = m_lens->getSensorElement(ix, iy);
	newEmitter->setSampleCount(m_samples);
	newEmitter->setPosition(sensorElement);
	newEmitter->setSampler(m_sampler);
	newEmitter->setLens(m_lens);

	return (CameraRayEmitter *)newEmitter;
}
