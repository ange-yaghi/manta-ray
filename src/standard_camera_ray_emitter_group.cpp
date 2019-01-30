#include <standard_camera_ray_emitter_group.h>

#include <standard_camera_ray_emitter.h>
#include <sampler_2d.h>

manta::StandardCameraRayEmitterGroup::StandardCameraRayEmitterGroup() {
}

manta::StandardCameraRayEmitterGroup::~StandardCameraRayEmitterGroup() {
}

void manta::StandardCameraRayEmitterGroup::createAllEmitters() {
	int nRays = m_resolutionX * m_resolutionY;
	float planeWidth = (m_resolutionX / (float)m_resolutionY) * m_planeHeight;

	float xIncrement = planeWidth / m_resolutionX;
	float yIncrement = m_planeHeight / m_resolutionY;

	initializeEmitters(nRays);

	math::Vector right = math::cross(m_direction, m_up);
	math::Vector planeCenter = math::add(m_position, math::mul(m_direction, math::loadScalar(m_planeDistance)));

	m_sampler->setAxis1(right);
	m_sampler->setAxis2(m_up);
	m_sampler->setBoundaryHeight(yIncrement);
	m_sampler->setBoundaryWidth(xIncrement);

	for (int i = 0; i < m_resolutionY; i++) {
		for (int j = 0; j < m_resolutionX; j++) {
			StandardCameraRayEmitter *newEmitter = createEmitter<StandardCameraRayEmitter>();
			m_rayEmitters[i * m_resolutionX + j] = (CameraRayEmitter *)newEmitter;

			float x = xIncrement * j;
			float y = yIncrement * i;

			math::Vector loc = math::mul(right, math::loadScalar(x - planeWidth / 2.0f));
			loc = math::sub(loc, math::mul(m_up, math::loadScalar(y - m_planeHeight / 2.0f)));
			loc = math::add(loc, planeCenter);

			newEmitter->setPosition(m_position);
			newEmitter->setTargetOrigin(loc);
			newEmitter->setSampler(m_sampler);
			newEmitter->setSampleCount(m_samples);
		}
	}
}
