#include <material.h>

#include <ray_emitter_group.h>
#include <intersection_point.h>
#include <light_ray.h>
#include <simple_ray_emitter.h>
#include <simple_ray_emitter_group.h>
#include <stack_allocator.h>

manta::Material::Material() {

}

manta::Material::~Material() {

}

manta::RayEmitterGroup *manta::Material::generateRayEmitters(const LightRay *ray, const IntersectionPoint *intersectionPoint, int degree, StackAllocator *stackAllocator) const {
	RayEmitterGroup *newGroup = generateRayEmittersInternal(ray, intersectionPoint, degree, stackAllocator);
	
	return newGroup;
}

manta::RayEmitterGroup *manta::Material::generateRayEmittersInternal(const LightRay *ray, const IntersectionPoint *intersectionPoint, int degree, StackAllocator *stackAllocator) const {
	if (degree >= 20) {
		return nullptr;
	}

	if (degree >= 2) {
		int a = 0;
	}

	// Calculate bias point
	math::Vector biasPoint = math::add(intersectionPoint->m_position, math::mul(intersectionPoint->m_normal, math::loadScalar(0.001f)));

	math::Vector perturb = intersectionPoint->m_normal;
	math::Vector n_dot_d = math::dot(intersectionPoint->m_normal, ray->getDirection());
	perturb = math::mul(perturb, math::add(n_dot_d, n_dot_d)); // Multiply by 2
	math::Vector finalDirection = math::sub(ray->getDirection(), perturb);
	finalDirection = math::normalize(finalDirection);

	SimpleRayEmitterGroup *newEmitter = createEmitterGroup<SimpleRayEmitterGroup>(degree, stackAllocator);
	newEmitter->m_simpleRayEmitter->setDirection(finalDirection);
	newEmitter->m_simpleRayEmitter->setPosition(biasPoint);

	return newEmitter;
}

void manta::Material::integrateRay(LightRay *ray, const RayEmitterGroup *_rayEmitter) const {
	SimpleRayEmitterGroup *rayEmitter = (SimpleRayEmitterGroup *)_rayEmitter;
	LightRay *reflectedRay = rayEmitter->m_simpleRayEmitter->getRay();

	ray->setIntensity(math::add(math::mul(reflectedRay->getIntensity(), math::loadScalar(0.5)), m_emission));

	math::real r1 = math::getX(ray->getIntensity());
	math::real r2 = math::getX(m_emission);
	if (r2 > r1 && _rayEmitter->getDegree() == 1) {
		int a = 0;
	}
	//ray->setIntensity(m_emission);
}

void manta::Material::destroyEmitterGroup(RayEmitterGroup *group, StackAllocator *stackAllocator) {
	
	group->destroyEmitters();
	
	group->~RayEmitterGroup();

	if (stackAllocator == nullptr) {
		_aligned_free((void *)group);
	}
	else {
		stackAllocator->free((void *)group);
	}
}
