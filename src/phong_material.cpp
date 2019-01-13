#include <phong_material.h>

#include <hemi_monte_carlo_emitter.h>
#include <hemi_monte_carlo_emitter_group.h>

#include <intersection_point.h>
#include <light_ray.h>

manta::PhongMaterial::PhongMaterial() {
	m_maxDegree = 4;
}

manta::PhongMaterial::~PhongMaterial() {
}

void manta::PhongMaterial::integrateRay(LightRay *ray, const RayEmitterGroup *_rayEmitter, const IntersectionPoint *intersectionPoint) const {
	math::Vector totalLight = m_emission;
	
	if (_rayEmitter != nullptr) {
		HemiMonteCarloEmitterGroup *group = (HemiMonteCarloEmitterGroup *)_rayEmitter;
		HemiMonteCarloEmitter *emitter = group->getHemiEmitter(0);

		if (emitter->getMeta() == SPECULAR_EMITTER) {
			// Calculate reflection vector
			math::Vector perturb = intersectionPoint->m_vertexNormal;
			math::Vector n_dot_d = math::dot(intersectionPoint->m_vertexNormal, ray->getDirection());
			perturb = math::mul(perturb, math::add(n_dot_d, n_dot_d)); // Multiply by 2
			math::Vector reflection = math::sub(perturb, ray->getDirection());

			const LightRay &ray = emitter->getRays()[0];
			math::Vector k_spec = math::dot(ray.getDirection(), reflection);

			math::real k_spec_r = math::getScalar(k_spec);
			k_spec_r = pow(k_spec_r, m_specularPower);

			math::Vector light = math::mul(math::loadScalar(k_spec_r), ray.getIntensity());

			totalLight = math::add(totalLight,
				math::mul(light, m_specularColor));
		}
		else if (emitter->getMeta() == DIFFUSE_EMITTER) {
			totalLight = math::add(totalLight,
				math::mul(m_diffuseColor, emitter->getIntensity()));
		}
	}

	ray->setIntensity(totalLight);
}

void manta::PhongMaterial::preconfigureEmitterGroup(RayEmitterGroup *_group, int degree) const {
	HemiMonteCarloEmitterGroup *group = static_cast<HemiMonteCarloEmitterGroup *>(_group);
	group->setLayerCount(1);
}

manta::RayEmitterGroup *manta::PhongMaterial::generateRayEmittersInternal(const LightRay *ray, const IntersectionPoint *intersectionPoint, int degree, StackAllocator *stackAllocator) const {
	if (degree >= m_maxDegree) {
		return nullptr;
	}

	HemiMonteCarloEmitterGroup *group = createEmitterGroup<HemiMonteCarloEmitterGroup>(degree, stackAllocator);
	HemiMonteCarloEmitter *emitter = group->getHemiEmitter(0);
	emitter->setNormal(intersectionPoint->m_vertexNormal);
	emitter->setPosition(intersectionPoint->m_position);
	emitter->setSampleCount(1);

	math::real r = math::uniformRandom();

	if (r >= 0.5) {
		emitter->setMeta(SPECULAR_EMITTER);
	}
	else {
		emitter->setMeta(DIFFUSE_EMITTER);
	}

	return static_cast<RayEmitterGroup *>(group);
}
