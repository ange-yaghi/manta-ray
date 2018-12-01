#include "simple_specular_diffuse_material.h"

#include <light_ray.h>
#include <intersection_point.h>
#include <monte_carlo_specular_diffuse_group.h>

#include <iostream>

manta::SimpleSpecularDiffuseMaterial::SimpleSpecularDiffuseMaterial() {
	m_maxDiffuseDegree = 5;
	m_maxSpecularDegree = 5;

	m_autoDisableEmitters = true;
	m_enableDiffuse = true;
	m_enableSpecular = true;
}

manta::SimpleSpecularDiffuseMaterial::~SimpleSpecularDiffuseMaterial() {
}

void manta::SimpleSpecularDiffuseMaterial::integrateRay(LightRay * ray, const RayEmitterGroup * _rayEmitter) const {
	math::Vector addedLight = math::constants::Zero;

	if (_rayEmitter != nullptr) {
		MonteCarloSpecularDiffuseGroup *rayEmitter = (MonteCarloSpecularDiffuseGroup *)_rayEmitter;
		SimpleRayEmitter *specular = rayEmitter->m_specularEmitter;
		BatchedMonteCarloEmitter *diffuse = rayEmitter->m_diffuseEmitter;

		if (specular != nullptr) {
			LightRay *reflectedRay = specular->getRay();
			addedLight = math::add(addedLight, math::mul(m_specularColor, reflectedRay->getIntensity()));
		}

		if (diffuse != nullptr) {
			math::Vector ave = diffuse->getAverageIntensity();
			addedLight = math::add(addedLight, math::mul(m_diffuseColor, ave));

			//if (math::getX(diffuse->getAverageIntensity()) > 0.5) {

				//ray->setIntensity(
				//	math::add(math::mul(diffuse->getNormal(), math::constants::Half), math::loadVector(0.5, 0.5, 0.5))
				//);

				//std::cout << math::getX(diffuse->getRays()[0].getDirection()) << ", " << math::getY(diffuse->getRays()[0].getDirection()) << ", " << math::getZ(diffuse->getRays()[0].getDirection());
			//}
		}
	}

	ray->setIntensity(
			math::add(
			addedLight,
			m_emission
		)
	);
}

void manta::SimpleSpecularDiffuseMaterial::setSpecularColor(const math::Vector &specular) {
	m_specularColor = specular;

	if (m_autoDisableEmitters) {
		double magnitude = math::getScalar(math::magnitudeSquared3(specular));
		if (magnitude < 1e-4) {
			m_enableSpecular = false;
		}
		else {
			m_enableSpecular = true;
		}
	}
	else {
		m_enableSpecular = true;
	}
}

void manta::SimpleSpecularDiffuseMaterial::setDiffuseColor(const math::Vector &diffuse) {
	m_diffuseColor = diffuse;

	if (m_autoDisableEmitters) {
		double magnitude = math::getScalar(math::magnitudeSquared3(diffuse));
		if (magnitude < 1e-8) {
			m_enableDiffuse = false;
		}
		else {
			m_enableDiffuse = true;
		}
	}
	else {
		m_enableDiffuse = true;
	}
}

void manta::SimpleSpecularDiffuseMaterial::setAutoDisableEmitters(bool autoDisableEmitters) {
	m_autoDisableEmitters = autoDisableEmitters;

	if (!m_autoDisableEmitters) {
		m_enableDiffuse = true;
		m_enableSpecular = true;
	}
	else {
		setSpecularColor(m_specularColor);
		setDiffuseColor(m_diffuseColor);
	}
}

int manta::SimpleSpecularDiffuseMaterial::getDiffuseSampleCount(int degree) const {
	// TODO: add logic to change the sample count as the degree changes
	switch (degree) {
	case 1:
		return 1;
	case 2:
		return 1;
	case 3:
		return 1;
	case 4:
		return 1;
	case 5:
		return 1;
	default:
		return 0;
	}
}

void manta::SimpleSpecularDiffuseMaterial::preconfigureEmitterGroup(RayEmitterGroup * _group, int degree) const {
	MonteCarloSpecularDiffuseGroup *group = (MonteCarloSpecularDiffuseGroup *)_group;

	int diffuseSamples = getDiffuseSampleCount(degree);

	if (degree >= m_maxDiffuseDegree || !m_enableDiffuse || diffuseSamples <= 0) group->setDiffuseEnabled(false);
	else group->setDiffuseEnabled(true);

	if (degree >= m_maxSpecularDegree || !m_enableSpecular) group->setSpecularEnabled(false);
	else group->setSpecularEnabled(true);
}

manta::RayEmitterGroup * manta::SimpleSpecularDiffuseMaterial::generateRayEmittersInternal(const LightRay * ray, const IntersectionPoint * intersectionPoint, int degree, StackAllocator *stackAllocator) const {
	if (degree >= m_maxDiffuseDegree && degree >= m_maxSpecularDegree) {
		return nullptr;
	}

	if (!m_enableDiffuse && !m_enableSpecular) return nullptr;

	MonteCarloSpecularDiffuseGroup *newEmitter = createEmitterGroup<MonteCarloSpecularDiffuseGroup>(degree, stackAllocator);

	int diffuseSamples = getDiffuseSampleCount(degree);

	if (degree < m_maxDiffuseDegree && m_enableDiffuse && diffuseSamples > 0) {
		newEmitter->m_diffuseEmitter->setNormal(intersectionPoint->m_vertexNormal);
		newEmitter->m_diffuseEmitter->setIncident(ray->getDirection());
		newEmitter->m_diffuseEmitter->setPosition(intersectionPoint->m_position);
		newEmitter->m_diffuseEmitter->setSampleCount(diffuseSamples);
	}
	
	if (degree < m_maxSpecularDegree && m_enableSpecular) {
		math::Vector perturb = intersectionPoint->m_vertexNormal;
		math::Vector n_dot_d = math::dot(intersectionPoint->m_vertexNormal, ray->getDirection());
		perturb = math::mul(perturb, math::add(n_dot_d, n_dot_d)); // Multiply by 2
		math::Vector finalDirection = math::sub(ray->getDirection(), perturb);
		finalDirection = math::normalize(finalDirection);

		newEmitter->m_specularEmitter->setDirection(finalDirection);
		newEmitter->m_specularEmitter->setPosition(intersectionPoint->m_position);
	}

	return newEmitter;
}
