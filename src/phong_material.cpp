#include <phong_material.h>

#include <phong_emitter.h>
#include <phong_emitter_group.h>

#include <intersection_point.h>
#include <light_ray.h>

manta::PhongMaterial::PhongMaterial() {
	m_maxDegree = 5;
}

manta::PhongMaterial::~PhongMaterial() {
}

void manta::PhongMaterial::integrateRay(LightRay *ray, const RayEmitterGroup *_rayEmitter, const IntersectionPoint *intersectionPoint) const {
	math::Vector totalLight = m_emission;
	
	if (_rayEmitter != nullptr) {
		PhongEmitterGroup *group = (PhongEmitterGroup *)_rayEmitter;
		PhongEmitter *emitter = group->getPhongEmitter(0);

		LightRay *i = &emitter->getRays()[0];

		totalLight = math::add(totalLight,
			math::mul(
				i->getWeightedIntensity(),
				m_specularColor));

		//totalLight = math::add(math::mul(emitter->getNormal(), math::constants::Half), math::loadVector(0.5, 0.5, 0.5));
	}

	ray->setIntensity(totalLight);
}

void manta::PhongMaterial::preconfigureEmitterGroup(RayEmitterGroup *_group, int degree) const {
}

manta::RayEmitterGroup *manta::PhongMaterial::generateRayEmittersInternal(const LightRay *ray, const IntersectionPoint *intersectionPoint, int degree, StackAllocator *stackAllocator) const {
	if (degree >= m_maxDegree) {
		return nullptr;
	}

	PhongEmitterGroup *group = createEmitterGroup<PhongEmitterGroup>(degree, stackAllocator);
	PhongEmitter *emitter = group->getPhongEmitter(0);
	emitter->setFaceNormal(intersectionPoint->m_faceNormal);
	emitter->setVertexNormal(intersectionPoint->m_vertexNormal);
	emitter->setPosition(intersectionPoint->m_position);
	emitter->setIncident(ray->getDirection());
	emitter->setPower(m_specularPower);
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
