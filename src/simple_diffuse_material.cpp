#include <simple_diffuse_material.h>

#include <simple_monte_carlo_emitter_group.h>
#include <simple_monte_carlo_emitter.h>

#include <intersection_point.h>
#include <light_ray.h>

manta::SimpleDiffuseMaterial::SimpleDiffuseMaterial() {
	m_maxDegree = 4;
}

manta::SimpleDiffuseMaterial::~SimpleDiffuseMaterial() {

}

void manta::SimpleDiffuseMaterial::integrateRay(LightRay *ray, const RayEmitterGroup *_rayEmitter, const IntersectionPoint *intersectionPoint) const {
	SimpleMonteCarloEmitterGroup *rayEmitter = (SimpleMonteCarloEmitterGroup *)_rayEmitter;
	LightRay *reflectedRay = rayEmitter->m_simpleRayEmitter->getRay();

	ray->setIntensity(
		math::add(
			math::mul(reflectedRay->getWeightedIntensity(), m_diffuseColor),
			m_emission
		)
	);
}

manta::RayEmitterGroup * manta::SimpleDiffuseMaterial::generateRayEmittersInternal(const LightRay *ray, const IntersectionPoint *intersectionPoint, int degree, StackAllocator *stackAllocator) const {
	if (degree >= m_maxDegree) {
		return nullptr;
	}

	SimpleMonteCarloEmitterGroup *newEmitter = createEmitterGroup<SimpleMonteCarloEmitterGroup>(degree, stackAllocator);
	newEmitter->m_simpleRayEmitter->setNormal(intersectionPoint->m_vertexNormal);
	newEmitter->m_simpleRayEmitter->setIncident(ray->getDirection());
	newEmitter->m_simpleRayEmitter->setPosition(intersectionPoint->m_position);

	return newEmitter;
}
