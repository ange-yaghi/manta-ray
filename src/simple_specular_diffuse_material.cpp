#include "simple_specular_diffuse_material.h"

#include <light_ray.h>
#include <intersection_point.h>
#include <monte_carlo_specular_diffuse_group.h>

manta::SimpleSpecularDiffuseMaterial::SimpleSpecularDiffuseMaterial() {
	m_maxDiffuseDegree = 4;
	m_maxSpecularDegree = 5;
}

manta::SimpleSpecularDiffuseMaterial::~SimpleSpecularDiffuseMaterial() {
}

void manta::SimpleSpecularDiffuseMaterial::integrateRay(LightRay * ray, const RayEmitterGroup * _rayEmitter) const {
	MonteCarloSpecularDiffuseGroup *rayEmitter = (MonteCarloSpecularDiffuseGroup *)_rayEmitter;
	SimpleRayEmitter *specular = rayEmitter->m_specularEmitter;
	SimpleMonteCarloEmitter *diffuse = rayEmitter->m_diffuseEmitter;

	math::Vector addedLight = math::constants::Zero;

	if (specular != nullptr) {
		LightRay *reflectedRay = specular->getRay();
		addedLight = math::add(addedLight, math::mul(m_specularColor, reflectedRay->getIntensity()));
	}

	if (diffuse != nullptr) {
		LightRay *reflectedRay = diffuse->getRay();
		addedLight = math::add(addedLight, math::mul(m_diffuseColor, reflectedRay->getIntensity()));
	}

	ray->setIntensity(
		math::add(
			addedLight,
			m_emission
		)
	);
}

void manta::SimpleSpecularDiffuseMaterial::preconfigureEmitterGroup(RayEmitterGroup * _group, int degree) const {
	MonteCarloSpecularDiffuseGroup *group = (MonteCarloSpecularDiffuseGroup *)_group;
	if (degree >= m_maxDiffuseDegree) group->setDiffuseEnabled(false);
	else group->setDiffuseEnabled(true);

	if (degree >= m_maxSpecularDegree) group->setSpecularEnabled(false);
	else group->setSpecularEnabled(true);
}

manta::RayEmitterGroup * manta::SimpleSpecularDiffuseMaterial::generateRayEmittersInternal(const LightRay * ray, const IntersectionPoint * intersectionPoint, int degree) const {
	if (degree >= m_maxDiffuseDegree && degree >= m_maxSpecularDegree) {
		return nullptr;
	}

	// Calculate bias point
	math::Vector biasPoint = math::add(intersectionPoint->m_position, math::mul(intersectionPoint->m_normal, math::loadScalar(0.001f)));
	MonteCarloSpecularDiffuseGroup *newEmitter = createEmitterGroup<MonteCarloSpecularDiffuseGroup>(degree);

	if (degree < m_maxDiffuseDegree) {
		newEmitter->m_diffuseEmitter->setNormal(intersectionPoint->m_normal);
		newEmitter->m_diffuseEmitter->setIncident(ray->getDirection());
		newEmitter->m_diffuseEmitter->setPosition(biasPoint);
	}
	
	if (degree < m_maxSpecularDegree) {
		math::Vector perturb = intersectionPoint->m_normal;
		math::Vector n_dot_d = math::dot(intersectionPoint->m_normal, ray->getDirection());
		perturb = math::mul(perturb, math::add(n_dot_d, n_dot_d)); // Multiply by 2
		math::Vector finalDirection = math::sub(ray->getDirection(), perturb);
		finalDirection = math::normalize(finalDirection);

		newEmitter->m_specularEmitter->setDirection(finalDirection);
		newEmitter->m_specularEmitter->setPosition(biasPoint);
	}

	return newEmitter;
}
