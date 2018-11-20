#include <camera_ray_emitter_group.h>

manta::CameraRayEmitterGroup::CameraRayEmitterGroup() {

}

manta::CameraRayEmitterGroup::~CameraRayEmitterGroup() {

}

void manta::CameraRayEmitterGroup::createAllEmitters() {
	int nRays = m_resolutionX * m_resolutionY;
	float planeWidth = (m_resolutionX / (float)m_resolutionY) * m_planeHeight;

	float xIncrement = planeWidth / m_resolutionX;
	float yIncrement = m_planeHeight / m_resolutionY;

	initializeEmitters(nRays);

	math::Vector right = math::cross(m_direction, m_up);

	math::Vector perturbX; 
	math::Vector perturbY;

	if (m_samplingWidth == 1) {
		perturbX = math::constants::Zero;
		perturbY = math::constants::Zero;
	}
	else {
		perturbX = math::mul(math::loadScalar(xIncrement / (m_samplingWidth)), right);
		perturbY = math::mul(math::loadScalar(yIncrement / (m_samplingWidth)), m_up);
	}

	math::Vector planeCenter = math::add(m_position, math::mul(m_direction, math::loadScalar(m_planeDistance)));

	for (int i = 0; i < m_resolutionY; i++) {
		for (int j = 0; j < m_resolutionX; j++) {
			CameraRayEmitter *newEmitter = createEmitter<CameraRayEmitter>();
			m_rayEmitters[i * m_resolutionX + j] = (RayEmitter *)newEmitter;

			float x = xIncrement * j;
			float y = yIncrement * i;

			math::Vector loc = math::mul(right, math::loadScalar(x - planeWidth / 2.0f));
			loc = math::sub(loc, math::mul(m_up, math::loadScalar(y - m_planeHeight / 2.0f)));
			loc = math::add(loc, planeCenter);

			math::Vector direction = math::sub(loc, m_position);

			newEmitter->setStartDirection(direction);
			newEmitter->setSampleOffsetX(perturbX);
			newEmitter->setSampleOffsetY(perturbY);
			newEmitter->setAASamples(m_samplingWidth);
			newEmitter->setSamplesPerRay(m_samplesPerPixel);

			direction = math::normalize(direction);

			newEmitter->setDirection(direction);
			newEmitter->setPosition(m_position);
		}
	}
}