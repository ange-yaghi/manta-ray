#include <phong_material.h>

#include <phong_emitter.h>
#include <phong_emitter_group.h>
#include <hemi_monte_carlo_emitter.h>

#include <intersection_point.h>
#include <light_ray.h>

manta::PhongMaterial::PhongMaterial() {
	m_maxDegree = 5;
	m_surfaceTransmission = (math::real)0.5;
}

manta::PhongMaterial::~PhongMaterial() {
}

void manta::PhongMaterial::integrateRay(LightRay *ray, const RayEmitterGroup *_rayEmitter, const IntersectionPoint *intersectionPoint) const {
	math::Vector totalLight = m_emission;
	
	if (_rayEmitter != nullptr) {
		if (_rayEmitter->getMeta() == SPECULAR_EMITTER) {
			PhongEmitter *emitter = static_cast<PhongEmitter *>(_rayEmitter->getEmitter());

			LightRay *i = &emitter->getRays()[0];

			totalLight = math::add(totalLight,
				math::mul(
					i->getWeightedIntensity(),
					m_specularColor));
		}
		else if (_rayEmitter->getMeta() == DIFFUSE_EMITTER) {
			HemiMonteCarloEmitter *emitter = static_cast<HemiMonteCarloEmitter *>(_rayEmitter->getEmitter());

			LightRay *i = &emitter->getRays()[0];

			totalLight = math::add(totalLight,
				math::mul(
					i->getWeightedIntensity(),
					m_diffuseColor));
		}

		//totalLight = math::add(math::mul(emitter->getNormal(), math::constants::Half), math::loadVector(0.5, 0.5, 0.5));
	}

	ray->setIntensity(totalLight);
}

void manta::PhongMaterial::configureEmitterGroup(RayEmitterGroup *group, int degree, const LightRay *ray, const IntersectionPoint *intersectionPoint)  const {
	group->initializeEmitters(1);

	if (math::uniformRandom() <= m_surfaceTransmission) {
		group->setMeta(SPECULAR_EMITTER);
		PhongEmitter *emitter = group->createEmitter<PhongEmitter>();
		emitter->setFaceNormal(intersectionPoint->m_faceNormal);
		emitter->setVertexNormal(intersectionPoint->m_vertexNormal);
		emitter->setPosition(intersectionPoint->m_position);
		emitter->setIncident(ray->getDirection());
		emitter->setPower(m_specularPower);
		emitter->setSampleCount(1);
	}
	else {
		group->setMeta(DIFFUSE_EMITTER);
		HemiMonteCarloEmitter *emitter = group->createEmitter<HemiMonteCarloEmitter>();
		emitter->setNormal(intersectionPoint->m_vertexNormal);
		emitter->setPosition(intersectionPoint->m_position);
		emitter->setSamplesPerRay(1);
		emitter->setSampleCount(1);
	}
}

manta::RayEmitterGroup *manta::PhongMaterial::generateRayEmittersInternal(const LightRay *ray, const IntersectionPoint *intersectionPoint, int degree, StackAllocator *stackAllocator) const {
	if (degree >= m_maxDegree) {
		return nullptr;
	}

	RayEmitterGroup *group = createEmitterGroup<RayEmitterGroup>(degree, ray, intersectionPoint, stackAllocator);

	return static_cast<RayEmitterGroup *>(group);
}
