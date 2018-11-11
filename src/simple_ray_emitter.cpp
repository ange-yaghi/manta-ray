#include <simple_ray_emitter.h>

#include <light_ray.h>

manta::SimpleRayEmitter::SimpleRayEmitter() {

}

manta::SimpleRayEmitter::~SimpleRayEmitter() {

}

void manta::SimpleRayEmitter::generateRays() {
	initializeRays(1);

	LightRay *mainRay = getRays();
	mainRay->setDirection(getDirection());
	mainRay->setSource(getPosition());

	m_singleRay = mainRay;
}