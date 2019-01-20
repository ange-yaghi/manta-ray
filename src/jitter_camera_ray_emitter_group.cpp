#include <jitter_camera_ray_emitter_group.h>

#include <jitter_camera_ray_emitter.h>

manta::JitterCameraRayEmitterGroup::JitterCameraRayEmitterGroup() {
}

manta::JitterCameraRayEmitterGroup::~JitterCameraRayEmitterGroup() {
}

void manta::JitterCameraRayEmitterGroup::createAllEmitters() {
	int nRays = m_resolutionX * m_resolutionY;
	float planeWidth = (m_resolutionX / (float)m_resolutionY) * m_planeHeight;

	float xIncrement = planeWidth / m_resolutionX;
	float yIncrement = m_planeHeight / m_resolutionY;

	initializeEmitters(nRays);

	math::Vector right = math::cross(m_direction, m_up);

	math::Vector planeCenter = math::add(m_position, math::mul(m_direction, math::loadScalar(m_planeDistance)));

	for (int i = 0; i < m_resolutionY; i++) {
		for (int j = 0; j < m_resolutionX; j++) {
			JitterCameraRayEmitter *newEmitter = createEmitter<JitterCameraRayEmitter>();
			m_rayEmitters[i * m_resolutionX + j] = (RayEmitter *)newEmitter;

			float x = xIncrement * j;
			float y = yIncrement * i;

			math::Vector loc = math::mul(right, math::loadScalar(x - planeWidth / 2.0f));
			loc = math::sub(loc, math::mul(m_up, math::loadScalar(y - m_planeHeight / 2.0f)));
			loc = math::add(loc, planeCenter);

			newEmitter->setStartPosition(loc);
			newEmitter->setWidth(xIncrement);
			newEmitter->setHeight(yIncrement);
			newEmitter->setAxisX(right);
			newEmitter->setAxisY(m_up);
			newEmitter->setAASamples(m_samplesPerPixel);
			newEmitter->setSamplesPerRay(1);
			newEmitter->setWidth(xIncrement);
			newEmitter->setHeight(yIncrement);
			newEmitter->setPosition(m_position);
		}
	}
}
