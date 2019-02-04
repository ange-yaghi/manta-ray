#include <phong_phong_bilayer_material.h>

#include <ray_container.h>
#include <intersection_point.h>
#include <light_ray.h>
#include <vector_material_node.h>

manta::PhongPhongBilayerMaterial::PhongPhongBilayerMaterial() {
	m_maxDegree = 5;
	m_surfaceTransmission = (math::real)0.5;

	m_specularBSDF.setMediaInterface(&m_coatingFresnel);
}

manta::PhongPhongBilayerMaterial::~PhongPhongBilayerMaterial() {
}

void manta::PhongPhongBilayerMaterial::integrateRay(LightRay *ray, const RayContainer &rays, const IntersectionPoint &intersectionPoint) const {
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
			math::Vector diffuseColor = m_diffuseColor;

			if (m_diffuseNode != nullptr) {
				diffuseColor = m_diffuseNode->sample(&intersectionPoint);
			}

			totalLight = math::add(totalLight,
				math::mul(
					mainRay.getWeightedIntensity(),
					diffuseColor));
			//ray->setIntensity(math::loadVector(0.0, 1.0, 0.0));
		}
		//totalLight = math::loadScalar(mainRay.getWeight());
	}

	//totalLight = math::add(math::mul(intersectionPoint.m_vertexNormal, math::constants::Half), math::loadVector(0.5, 0.5, 0.5));
	ray->setIntensity(totalLight);
}

void manta::PhongPhongBilayerMaterial::generateRays(RayContainer *rays, const LightRay &incidentRay, const IntersectionPoint &intersectionPoint, int degree, StackAllocator *stackAllocator) const {
	if (degree > m_maxDegree) return;

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
	BSDFInput u_param;
	u_param.incident = incident;
	u_param.normal = intersectionPoint.m_vertexNormal;
	u_param.u = u;
	u_param.v = v;
	u_param.surfaceInteraction = &intersectionPoint;

	m_specularBSDF.initialize(&u_param, stackAllocator);

	math::Vector upper_m = m_specularBSDF.generateMicrosurfaceNormal(u_param);
	MediaInterface::DIRECTION upperRT = m_specularBSDF.decideDirection(u_param, upper_m, MediaInterface::DIRECTION_IN);

	math::Vector m, o;
	math::real weight = 1.0;

	LightRay &ray = rays->getRays()[0];

	if (upperRT == MediaInterface::DIRECTION_OUT) {
		// Ray has reflected off of the coating
		ray.setMeta(SPECULAR_EMITTER);
		m = upper_m;
		o = math::normalize(m_specularBSDF.reflectionDirection(u_param, upper_m));
		weight *= m_specularBSDF.generateWeight(u_param, upper_m, o);
	}
	else {
		// Ray has penetrated to the lower layer
		ray.setMeta(DIFFUSE_EMITTER);
		math::Vector o_transmission = m_specularBSDF.transmissionDirection(u_param, m_coatingFresnel.getIorIncident() / m_coatingFresnel.getIorTransmitted(), upper_m);
		weight *= m_specularBSDF.generateWeight(u_param, upper_m, o_transmission);

		math::Vector newIncident = math::negate(o_transmission);

		// Generate an outgoing direction from the lower layer
		BSDFInput l_param;
		l_param.incident = newIncident;
		l_param.normal = intersectionPoint.m_vertexNormal;
		l_param.u = u;
		l_param.v = v;
		l_param.surfaceInteraction = &intersectionPoint;

		m_diffuseBSDF.initialize(&l_param, stackAllocator);

		m = m_diffuseBSDF.generateMicrosurfaceNormal(l_param);
		o = m_diffuseBSDF.reflectionDirection(l_param, m);
		weight *= m_diffuseBSDF.generateWeight(l_param, m, o);

		m_diffuseBSDF.free(&l_param, stackAllocator);
	}

	constexpr math::real MAX_WEIGHT = (math::real)1000.0;

	if (weight > MAX_WEIGHT) {
		weight = MAX_WEIGHT;
	}

	// Initialize the outgoing ray
	ray.setDirection(o);
	ray.setWeight(weight);
	ray.setIntensity(math::constants::Zero);
	ray.setSource(intersectionPoint.m_position);
}
