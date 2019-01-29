#include <simple_phong_lambert_material.h>

#include <ray_emitter_group.h>
#include <bsdf_emitter.h>
#include <phong_emitter.h>

#include <intersection_point.h>
#include <light_ray.h>

manta::SimplePhongLambertMaterial::SimplePhongLambertMaterial() {
	m_maxDegree = 5;
	m_surfaceTransmission = (math::real)0.5;
}

manta::SimplePhongLambertMaterial::~SimplePhongLambertMaterial() {
}

void manta::SimplePhongLambertMaterial::integrateRay(LightRay *ray, const RayEmitterGroup *_rayEmitter, const IntersectionPoint *intersectionPoint) const {
	math::Vector totalLight = m_emission;
	
	if (_rayEmitter != nullptr) {
		if (_rayEmitter->getMeta() == SPECULAR_EMITTER) {
			BSDFEmitter *emitter = static_cast<BSDFEmitter *>(_rayEmitter->getEmitter());

			LightRay *i = &emitter->getRays()[0];

			totalLight = math::add(totalLight,
				math::mul(
					i->getWeightedIntensity(),
					m_specularColor));

			//ray->setIntensity(math::loadVector(1.0, 0.0, 0.0));
		}
		else if (_rayEmitter->getMeta() == DIFFUSE_EMITTER) {
			BSDFEmitter *emitter = static_cast<BSDFEmitter *>(_rayEmitter->getEmitter());

			LightRay *i = &emitter->getRays()[0];

			totalLight = math::add(totalLight,
				math::mul(
					i->getWeightedIntensity(),
					m_diffuseColor));
			//ray->setIntensity(math::loadVector(0.0, 1.0, 0.0));
		}
	}

	//totalLight = math::add(math::mul(intersectionPoint->m_vertexNormal, math::constants::Half), math::loadVector(0.5, 0.5, 0.5));

	ray->setIntensity(totalLight);
}

void manta::SimplePhongLambertMaterial::configureEmitterGroup(RayEmitterGroup *group, int degree, const LightRay *ray, const IntersectionPoint *intersectionPoint)  const {
	group->initializeEmitters(1);

	// Generate basis vectors
	math::Vector u = math::constants::YAxis;
	math::Vector v;
	math::Vector w = intersectionPoint->m_vertexNormal;
	if (abs(math::getX(intersectionPoint->m_vertexNormal)) < 0.1f) {
		u = math::constants::XAxis;
	}
	u = math::normalize(math::cross(u, intersectionPoint->m_vertexNormal));
	v = math::cross(intersectionPoint->m_vertexNormal, u);

	// Try upper layer first
	math::Vector upper_m = m_specularBSDF.generateMicrosurfaceNormal(intersectionPoint->m_vertexNormal, ray->getDirection(), u, v);
	MediaInterface::DIRECTION upperRT = m_specularBSDF.decideDirection(ray->getDirection(), upper_m, MediaInterface::DIRECTION_IN);

	if (upperRT == MediaInterface::DIRECTION_OUT) {
		group->setMeta(SPECULAR_EMITTER);
		BSDFEmitter *emitter = group->createEmitter<BSDFEmitter>();
		emitter->setNormal(intersectionPoint->m_vertexNormal);
		//emitter->setVertexNormal(intersectionPoint->m_vertexNormal);
		emitter->setPosition(intersectionPoint->m_position);
		emitter->setIncident(ray->getDirection());
		emitter->setBSDF(&m_specularBSDF);
		//emitter->setPower(m_specularBSDF.getPower());
		emitter->setSampleCount(1);
		return;
	}
	
	// Ray has penetrated to lower level
	group->setMeta(DIFFUSE_EMITTER);
	BSDFEmitter *emitter = group->createEmitter<BSDFEmitter>();
	emitter->setNormal(intersectionPoint->m_vertexNormal);
	emitter->setPosition(intersectionPoint->m_position);
	emitter->setIncident(ray->getDirection());
	emitter->setBSDF(&m_diffuseBSDF);
	emitter->setSampleCount(1);
}

manta::RayEmitterGroup *manta::SimplePhongLambertMaterial::generateRayEmittersInternal(const LightRay *ray, const IntersectionPoint *intersectionPoint, int degree, StackAllocator *stackAllocator) const {
	if (degree >= m_maxDegree) {
		return nullptr;
	}

	RayEmitterGroup *group = createEmitterGroup<RayEmitterGroup>(degree, ray, intersectionPoint, stackAllocator);

	return static_cast<RayEmitterGroup *>(group);
}
