#include <standard_camera_ray_emitter_group.h>

#include <standard_camera_ray_emitter.h>
#include <sampler_2d.h>

manta::StandardCameraRayEmitterGroup::StandardCameraRayEmitterGroup() {

}

manta::StandardCameraRayEmitterGroup::~StandardCameraRayEmitterGroup() {

}

void manta::StandardCameraRayEmitterGroup::initialize() {
	m_planeWidth = (m_resolutionX / (math::real)m_resolutionY) * m_planeHeight;

	m_xIncrement = m_planeWidth / m_resolutionX;
	m_yIncrement = m_planeHeight / m_resolutionY;

	m_right = math::cross(m_direction, m_up);
	m_planeCenter = math::add(m_position, math::mul(m_direction, math::loadScalar(m_planeDistance)));

	m_sampler->setAxis1(m_right);
	m_sampler->setAxis2(m_up);
	m_sampler->setBoundaryHeight(m_yIncrement);
	m_sampler->setBoundaryWidth(m_xIncrement);
}

manta::CameraRayEmitter *manta::StandardCameraRayEmitterGroup::createEmitter(int ix, int iy, StackAllocator *stackAllocator) const {
	StandardCameraRayEmitter *newEmitter = allocateEmitter<StandardCameraRayEmitter>(stackAllocator);

	math::real x = ix * m_xIncrement;
	math::real y = iy * m_yIncrement;

	math::Vector loc = math::mul(m_right, math::loadScalar(x - m_planeWidth / 2.0f));
	loc = math::sub(loc, math::mul(m_up, math::loadScalar(y - m_planeHeight / 2.0f)));
	loc = math::add(loc, m_planeCenter);

	newEmitter->setPosition(m_position);
	newEmitter->setTargetOrigin(loc);
	newEmitter->setSampler(m_sampler);
	newEmitter->setSampleCount(m_samples);

	return newEmitter;
}
