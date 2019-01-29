#include <simple_phong_lambert_material.h>

#include <ray_container.h>

#include <intersection_point.h>
#include <light_ray.h>

manta::SimplePhongLambertMaterial::SimplePhongLambertMaterial() {
	m_maxDegree = 5;
	m_surfaceTransmission = (math::real)0.5;
}

manta::SimplePhongLambertMaterial::~SimplePhongLambertMaterial() {
}

void manta::SimplePhongLambertMaterial::integrateRay(LightRay *ray, const RayContainer &rays, const IntersectionPoint &intersectionPoint) const {
	math::Vector totalLight = m_emission;
	
	if (rays.getRayCount() > 0) {
		const LightRay &mainRay = rays.getRays()[0];

		if (mainRay.getMeta() == SPECULAR_EMITTER) {
			totalLight = math::add(totalLight,
				math::mul(
					mainRay.getWeightedIntensity(),
					m_specularColor));
			//ray->setIntensity(math::loadScalar(mainRay.getWeight()));
			//ray->setIntensity(math::loadVector(1.0, 0.0, 0.0));
		}
		else if (mainRay.getMeta() == DIFFUSE_EMITTER) {
			totalLight = math::add(totalLight,
				math::mul(
					mainRay.getWeightedIntensity(),
					m_diffuseColor));
			//ray->setIntensity(math::loadVector(0.0, 1.0, 0.0));
		}
	}

	//totalLight = math::add(math::mul(intersectionPoint.m_vertexNormal, math::constants::Half), math::loadVector(0.5, 0.5, 0.5));

	ray->setIntensity(totalLight);
}

void manta::SimplePhongLambertMaterial::generateRays(RayContainer *rays, const LightRay &incidentRay, const IntersectionPoint &intersectionPoint, int degree, StackAllocator *stackAllocator) const {
	if (degree >= 5) return;

	rays->initializeRays(1);

	// Generate basis vectors
	math::Vector u = math::constants::YAxis;
	math::Vector v;
	math::Vector w = intersectionPoint.m_vertexNormal;
	if (abs(math::getX(intersectionPoint.m_vertexNormal)) < 0.1f) {
		u = math::constants::XAxis;
	}
	u = math::normalize(math::cross(u, intersectionPoint.m_vertexNormal));
	v = math::cross(intersectionPoint.m_vertexNormal, u);

	math::Vector incident = math::negate(incidentRay.getDirection());

	// Try upper layer first
	math::Vector upper_m = m_specularBSDF.generateMicrosurfaceNormal(intersectionPoint.m_vertexNormal, incident, u, v);
	MediaInterface::DIRECTION upperRT = m_specularBSDF.decideDirection(incident, upper_m, MediaInterface::DIRECTION_IN);

	math::Vector m, o;
	math::real weight = 1.0;

	LightRay &ray = rays->getRays()[0];

	if (upperRT == MediaInterface::DIRECTION_OUT) {
		// Ray has reflected off of the coating
		ray.setMeta(SPECULAR_EMITTER);
		m = upper_m;
		o = m_specularBSDF.reflectionDirection(incident, upper_m);
		weight *= m_specularBSDF.generateWeight(intersectionPoint.m_vertexNormal, incident, upper_m, o);
	}
	else {
		// Ray has penetrated to the lower layer
		ray.setMeta(DIFFUSE_EMITTER);
		math::Vector o_transmission = m_specularBSDF.transmissionDirection(m_coatingFresnel.getIorIncident() / m_coatingFresnel.getIorTransmitted(), incident, upper_m, intersectionPoint.m_vertexNormal);
		weight *= m_specularBSDF.generateWeight(intersectionPoint.m_vertexNormal, incident, upper_m, o_transmission);

		math::Vector newIncident = math::negate(o_transmission);
		//newIncident = incident;
		m = m_diffuseBSDF.generateMicrosurfaceNormal(intersectionPoint.m_vertexNormal, newIncident, u, v);
		o = m_diffuseBSDF.reflectionDirection(newIncident, m);
		weight *= m_diffuseBSDF.generateWeight(intersectionPoint.m_vertexNormal, newIncident, m, o);
	}

	constexpr math::real MAX_WEIGHT = (math::real)2.0;

	if (weight > MAX_WEIGHT) {
		weight = MAX_WEIGHT;
	}

	// Initialize the outgoing ray
	ray.setDirection(o);
	ray.setWeight(weight);
	//ray.setWeight(1.0);
	ray.setIntensity(math::constants::Zero);
	ray.setSource(intersectionPoint.m_position);
}
