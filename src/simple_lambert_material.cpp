#include <simple_lambert_material.h>

#include <ray_container.h>

#include <intersection_point.h>
#include <light_ray.h>
#include <vector_material_node.h>


manta::SimpleLambertMaterial::SimpleLambertMaterial() {
	m_maxDegree = 5;

	m_diffuseNode = nullptr;

	//m_distribution.setPower((math::real)4.0);
	//m_diffuseBSDF.setDistribution(&m_distribution);
}

manta::SimpleLambertMaterial::~SimpleLambertMaterial() {
}

void manta::SimpleLambertMaterial::integrateRay(LightRay *ray, const RayContainer &rays, const IntersectionPoint &intersectionPoint) const {
	math::Vector totalLight = m_emission;

	math::Vector diffuseColor = m_diffuseColor;

	if (m_diffuseNode != nullptr) {
		diffuseColor = m_diffuseNode->sample(&intersectionPoint);
	}
	//totalLight = math::constants::Zero;
	if (rays.getRayCount() > 0) {
		const LightRay &mainRay = rays.getRays()[0];
		totalLight = math::add(totalLight,
			math::mul(
				mainRay.getWeightedIntensity(),
				diffuseColor));
			//ray->setIntensity(math::loadVector(0.0, 1.0, 0.0));
		//totalLight = mainRay.getIntensity();
	}

	//totalLight = math::add(math::mul(intersectionPoint->m_vertexNormal, math::constants::Half), math::loadVector(0.5, 0.5, 0.5));

	ray->setIntensity(totalLight);
}

void manta::SimpleLambertMaterial::generateRays(RayContainer *rays, const LightRay &incidentRay, const IntersectionPoint &intersectionPoint, int degree, StackAllocator *stackAllocator) const {
	if (degree > m_maxDegree) return;
	if (math::getScalar(math::magnitudeSquared3(m_diffuseColor)) < (math::real)1E-6 && m_diffuseNode == nullptr) return; /* Early exit if the diffuse color is black */

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

	LightRay &ray = rays->getRays()[0];
	math::Vector m, o;
	math::Vector weight;

	/*
	BSDFInput b_in;
	b_in.incident = incidentRay.getDirection();
	b_in.normal = intersectionPoint.m_vertexNormal;
	b_in.surfaceInteraction = &intersectionPoint;
	b_in.u = u;
	b_in.v = v;

	m_diffuseBSDF.initialize(&b_in, stackAllocator);

	m = m_diffuseBSDF.generateMicrosurfaceNormal(b_in);
	o = m_diffuseBSDF.reflectionDirection(b_in, m);
	weight = m_diffuseBSDF.generateWeight(b_in, m, o);

	m_diffuseBSDF.free(&b_in, stackAllocator);

	constexpr math::real MAX_WEIGHT = (math::real)2.0;

	if (weight > MAX_WEIGHT) {
		weight = MAX_WEIGHT;
	}
	*/

	math::Vector incident = math::negate(incidentRay.getDirection());

	// Transform incident ray
	math::Vector t_dir = math::loadVector(
		math::getScalar(math::dot(incident, u)),
		math::getScalar(math::dot(incident, v)),
		math::getScalar(math::dot(incident, intersectionPoint.m_vertexNormal)));

	math::Vector outgoing_t;
	math::real pdf;
	math::Vector reflectance = m_bsdf->sampleF(&intersectionPoint, t_dir, &outgoing_t, &pdf, stackAllocator);

	math::Vector outgoing = math::add(
		math::mul(u, math::loadScalar(math::getX(outgoing_t))),
		math::mul(v, math::loadScalar(math::getY(outgoing_t)))
	);
	outgoing = math::add(
		outgoing,
		math::mul(math::loadScalar(math::getZ(outgoing_t)), intersectionPoint.m_vertexNormal)
	);

	if (std::isnan(math::getX(outgoing))) {
		std::cout << "NAN" << std::endl;
		pdf = 0;
	}

	if (math::getX(outgoing) == 0.0 && math::getY(outgoing) == 0.0 && math::getZ(outgoing) == 0.0) {
		std::cout << "HELLO" << std::endl;
		int a = 0;
		std::cout << math::getX(outgoing_t) << ", " << math::getY(outgoing_t) << ", " << math::getZ(outgoing_t) << std::endl;
		if (math::getX(outgoing_t) == 0.0 && math::getY(outgoing_t) == 0.0 && math::getZ(outgoing_t) == 0.0) {
			std::cout << "HELLO2" << std::endl;

			int a = 0;
		}
	}

	if (pdf > 0) {
		weight = math::mul(reflectance, math::abs(math::dot(outgoing, intersectionPoint.m_vertexNormal)));
		weight = math::div(weight, math::loadScalar(pdf));
		//weight = (math::real)1.0;
	} 
	else {
		outgoing = math::loadVector(1, 0, 0);
		weight = math::constants::Zero;
	}

	if (std::isnan(math::getX(weight)) || std::isinf(math::getX(weight))) {
		std::cout << "NAN" << std::endl;
	}

	// Initialize the outgoing ray
	ray.setDirection(outgoing);
	ray.setWeight(weight);
	ray.setIntensity(math::constants::Zero);
	ray.setSource(intersectionPoint.m_position);
}
