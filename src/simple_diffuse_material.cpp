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

void manta::SimpleDiffuseMaterial::integrateRay(LightRay *ray, const RayEmitterGroup *_rayEmitter) const {
	SimpleMonteCarloEmitterGroup *rayEmitter = (SimpleMonteCarloEmitterGroup *)_rayEmitter;
	LightRay *reflectedRay = rayEmitter->m_simpleRayEmitter->getRay();

	ray->setIntensity(
		math::add(
			math::mul(reflectedRay->getIntensity(), m_diffuseColor),
			m_emission
		)
	);
}

manta::RayEmitterGroup * manta::SimpleDiffuseMaterial::generateRayEmittersInternal(const LightRay *ray, const IntersectionPoint *intersectionPoint, int degree, StackAllocator *stackAllocator) const {
	if (degree >= m_maxDegree) {
		return nullptr;
	}

	// Calculate bias point
	math::Vector biasPoint = math::add(intersectionPoint->m_position, math::mul(intersectionPoint->m_normal, math::loadScalar(0.001f)));

	SimpleMonteCarloEmitterGroup *newEmitter = createEmitterGroup<SimpleMonteCarloEmitterGroup>(degree, stackAllocator);
	newEmitter->m_simpleRayEmitter->setNormal(intersectionPoint->m_normal);
	newEmitter->m_simpleRayEmitter->setIncident(ray->getDirection());
	newEmitter->m_simpleRayEmitter->setPosition(biasPoint);

	return newEmitter;
}
