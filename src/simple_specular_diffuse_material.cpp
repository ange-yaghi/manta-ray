#include "simple_specular_diffuse_material.h"

#include <light_ray.h>
#include <intersection_point.h>
#include <monte_carlo_specular_diffuse_group.h>
#include <texture_map.h>

#include <iostream>

manta::SimpleSpecularDiffuseMaterial::SimpleSpecularDiffuseMaterial() {
	m_maxDiffuseDegree = 5;
	m_maxSpecularDegree = 5;

	m_autoDisableEmitters = true;
	m_enableDiffuse = true;
	m_enableSpecular = true;

	m_diffuseMap = nullptr;

	m_gloss = (math::real)1.0;
}

manta::SimpleSpecularDiffuseMaterial::~SimpleSpecularDiffuseMaterial() {
}

void manta::SimpleSpecularDiffuseMaterial::integrateRay(LightRay *ray, const RayEmitterGroup *_rayEmitter) const {
	math::Vector addedLight = math::constants::Zero;

	if (_rayEmitter != nullptr) {
		MonteCarloSpecularDiffuseGroup *rayEmitter = (MonteCarloSpecularDiffuseGroup *)_rayEmitter;
		BatchedMonteCarloEmitter *specular = rayEmitter->m_specularEmitter;
		BatchedMonteCarloEmitter *diffuse = rayEmitter->m_diffuseEmitter;

		if (specular != nullptr) {
			addedLight = math::add(addedLight, math::mul(m_specularColor, specular->getIntensity()));
		}

		if (diffuse != nullptr) {
			math::Vector diffuseColor = m_diffuseColor;
			math::Vector ave = diffuse->getIntensity();

			if (m_diffuseMap != nullptr) {
				math::Vector uv = rayEmitter->getTexCoord();
				diffuseColor = math::mul(diffuseColor, m_diffuseMap->sample(math::getX(uv), math::getY(uv)));
				//std::cout << math::getX(diffuseColor) << "," << math::getY(diffuseColor) << "," << math::getZ(diffuseColor) << std::endl;

				//ray->setIntensity(m_diffuseMap->sample(0.5, 0.45));
			}

			addedLight = math::add(addedLight, math::mul(diffuseColor, ave));


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

void manta::SimpleSpecularDiffuseMaterial::preconfigureEmitterGroup(RayEmitterGroup *_group, int degree) const {
	MonteCarloSpecularDiffuseGroup *group = (MonteCarloSpecularDiffuseGroup *)_group;

	int diffuseSamples = getDiffuseSampleCount(degree);

	if (degree >= m_maxDiffuseDegree || !m_enableDiffuse || diffuseSamples <= 0) group->setDiffuseEnabled(false);
	else group->setDiffuseEnabled(true);

	if (degree >= m_maxSpecularDegree || !m_enableSpecular) group->setSpecularEnabled(false);
	else group->setSpecularEnabled(true);
}

manta::RayEmitterGroup * manta::SimpleSpecularDiffuseMaterial::generateRayEmittersInternal(const LightRay *ray, const IntersectionPoint *intersectionPoint, int degree, StackAllocator *stackAllocator) const {
	if (degree >= m_maxDiffuseDegree && degree >= m_maxSpecularDegree) {
		return nullptr;
	}

	if (!m_enableDiffuse && !m_enableSpecular) return nullptr;

	MonteCarloSpecularDiffuseGroup *newEmitter = createEmitterGroup<MonteCarloSpecularDiffuseGroup>(degree, stackAllocator);
	newEmitter->setTexCoord(intersectionPoint->m_textureCoodinates);

	int diffuseSamples = getDiffuseSampleCount(degree);

	if (degree < m_maxDiffuseDegree && m_enableDiffuse && diffuseSamples > 0) {
		newEmitter->m_diffuseEmitter->setNormal(intersectionPoint->m_vertexNormal);
		newEmitter->m_diffuseEmitter->setIncident(ray->getDirection());
		newEmitter->m_diffuseEmitter->setPosition(intersectionPoint->m_position);
		newEmitter->m_diffuseEmitter->setSampleCount(diffuseSamples);
		newEmitter->m_diffuseEmitter->setGloss((math::real)0.0);
	}
	
	if (degree < m_maxSpecularDegree && m_enableSpecular) {
		newEmitter->m_specularEmitter->setNormal(intersectionPoint->m_vertexNormal);
		newEmitter->m_specularEmitter->setIncident(ray->getDirection());
		newEmitter->m_specularEmitter->setPosition(intersectionPoint->m_position);
		newEmitter->m_specularEmitter->setSampleCount(diffuseSamples);
		newEmitter->m_specularEmitter->setGloss((math::real)m_gloss);
	}

	return newEmitter;
}
