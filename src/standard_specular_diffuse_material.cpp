#include <standard_specular_diffuse_material.h>

#include <light_ray.h>
#include <intersection_point.h>
#include <batched_monte_carlo_emitter_group.h>
#include <texture_map.h>

#include <iostream>

manta::StandardSpecularDiffuseMaterial::StandardSpecularDiffuseMaterial() {
	m_diffuseMap = nullptr;
	m_maxDegree = 5;

	m_gloss = (math::real)1.0;
}

manta::StandardSpecularDiffuseMaterial::~StandardSpecularDiffuseMaterial() {
}

void manta::StandardSpecularDiffuseMaterial::integrateRay(LightRay *ray, const RayEmitterGroup *_rayEmitter, const IntersectionPoint *intersectionPoint) const {
	math::Vector addedLight = math::constants::Zero;

	if (_rayEmitter != nullptr) {
		BatchedMonteCarloEmitterGroup *rayEmitter = (BatchedMonteCarloEmitterGroup *)_rayEmitter;
		BatchedMonteCarloEmitter *emitter = rayEmitter->m_simpleRayEmitter;

		if (emitter->getMeta() == 0) {
			addedLight = math::add(addedLight, math::mul(m_diffuseColor, emitter->getIntensity()));

			//ray->setIntensity(
			//	math::add(math::mul(emitter->getNormal(), math::constants::Half), math::loadVector(0.5, 0.5, 0.5))
			//);
		}
		else if (emitter->getMeta() == 1) {
			math::Vector diffuseColor = m_diffuseColor;
			math::Vector ave = emitter->getIntensity();

			if (m_diffuseMap != nullptr) {
				math::Vector uv = rayEmitter->getTexCoord();
				diffuseColor = math::mul(diffuseColor, m_diffuseMap->sample(math::getX(uv), math::getY(uv)));
				//std::cout << math::getX(diffuseColor) << "," << math::getY(diffuseColor) << "," << math::getZ(diffuseColor) << std::endl;

				//ray->setIntensity(m_diffuseMap->sample(0.5, 0.45));
			}

			addedLight = math::add(addedLight, math::mul(diffuseColor, ave));


			//if (math::getX(diffuse->getAverageIntensity()) > 0.5) {

				//ray->setIntensity(
				//	math::add(math::mul(emitter->getNormal(), math::constants::Half), math::loadVector(0.5, 0.5, 0.5))
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

void manta::StandardSpecularDiffuseMaterial::preconfigureEmitterGroup(RayEmitterGroup *_group, int degree) const {
}

manta::RayEmitterGroup * manta::StandardSpecularDiffuseMaterial::generateRayEmittersInternal(const LightRay *ray, const IntersectionPoint *intersectionPoint, int degree, StackAllocator *stackAllocator) const {
	if (degree >= m_maxDegree) {
		return nullptr;
	}

	BatchedMonteCarloEmitterGroup *newEmitter = createEmitterGroup<BatchedMonteCarloEmitterGroup>(degree, stackAllocator);
	newEmitter->setTexCoord(intersectionPoint->m_textureCoodinates);

	math::real r = math::uniformRandom();

	if (r >= m_specular) {
		// Diffuse reflection
		newEmitter->m_simpleRayEmitter->setNormal(intersectionPoint->m_vertexNormal);
		newEmitter->m_simpleRayEmitter->setIncident(ray->getDirection());
		newEmitter->m_simpleRayEmitter->setPosition(intersectionPoint->m_position);
		newEmitter->m_simpleRayEmitter->setSampleCount(1);
		newEmitter->m_simpleRayEmitter->setGloss((math::real)0.0);
		newEmitter->m_simpleRayEmitter->setMeta(1);
	}
	else {
		// Specular reflection
		math::Vector perturb = intersectionPoint->m_vertexNormal;
		math::Vector n_dot_d = math::dot(intersectionPoint->m_vertexNormal, ray->getDirection());
		perturb = math::mul(perturb, math::add(n_dot_d, n_dot_d)); // Multiply by 2
		math::Vector normal = math::sub(perturb, ray->getDirection());
		//normal = math::negate(math::normalize(normal));

		newEmitter->m_simpleRayEmitter->setNormal(normal);
		newEmitter->m_simpleRayEmitter->setIncident(ray->getDirection());
		newEmitter->m_simpleRayEmitter->setPosition(intersectionPoint->m_position);
		newEmitter->m_simpleRayEmitter->setSampleCount(1);
		newEmitter->m_simpleRayEmitter->setGloss((math::real)m_gloss);
		newEmitter->m_simpleRayEmitter->setMeta(0);
	}

	return newEmitter;
}
