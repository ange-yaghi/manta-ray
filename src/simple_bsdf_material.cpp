#include "../include/simple_bsdf_material.h"

#include "../include/ray_container.h"
#include "../include/intersection_point.h"
#include "../include/light_ray.h"
#include "../include/vector_node_output.h"
#include "../include/bsdf.h"

manta::SimpleBSDFMaterial::SimpleBSDFMaterial() {
	m_maxDegree = 4;

	m_emissionNode = nullptr;
	m_reflectanceNode = nullptr;

	m_bsdf = nullptr;
    m_defaultBsdf = nullptr;

	m_reflectance = math::constants::One;
	m_emission = math::constants::One;
}

manta::SimpleBSDFMaterial::~SimpleBSDFMaterial() {
	/* void */
}

void manta::SimpleBSDFMaterial::integrateRay(LightRay *ray, const RayContainer &rays, const IntersectionPoint &intersectionPoint) const {
	math::Vector emission = m_emission;
	math::Vector reflectance = m_reflectance;

	if (m_emissionNode != nullptr) {
		math::Vector e;
		getEmissionNode()->sample(&intersectionPoint, (void *)&e);

		emission = math::mul(emission, e);
	}

	if (m_reflectanceNode != nullptr) {
		math::Vector r;
		getReflectanceNode()->sample(&intersectionPoint, (void *)&r);

		reflectance = math::mul(reflectance, r);
	}
	
	math::Vector totalLight = emission;

	if (rays.getRayCount() > 0) {
		const LightRay &mainRay = rays.getRays()[0];
		totalLight = math::add(totalLight,
			math::mul(
				mainRay.getWeightedIntensity(),
				reflectance));
	}

	ray->setIntensity(totalLight);
	//ray->setIntensity(math::mul(math::constants::Half, math::add(intersectionPoint.m_vertexNormal, math::constants::One)));
    //ray->setIntensity(emission);
}

const manta::VectorNodeOutput *manta::SimpleBSDFMaterial::getReflectanceNode() const {
	return static_cast<manta::VectorNodeOutput *>(m_reflectanceNode);
}

const manta::VectorNodeOutput *manta::SimpleBSDFMaterial::getEmissionNode() const {
	return static_cast<manta::VectorNodeOutput *>(m_emissionNode);
}

void manta::SimpleBSDFMaterial::_evaluate() {
    Material::_evaluate();

    m_bsdf = static_cast<ObjectReferenceNodeOutput<BSDF> *>(m_bsdfInput)->getReference();
}

void manta::SimpleBSDFMaterial::_initialize() {
	/* void */
}

void manta::SimpleBSDFMaterial::registerInputs() {
	Material::registerInputs();
	registerInput(&m_reflectanceNode, "reflectance");
	registerInput(&m_emissionNode, "emission");
	registerInput(&m_bsdfInput, "bsdf");
}

void manta::SimpleBSDFMaterial::generateRays(RayContainer *rays, const LightRay &incidentRay, const IntersectionPoint &intersectionPoint, int degree, StackAllocator *stackAllocator) const {
	if (degree > m_maxDegree) return;
	if (math::getScalar(math::magnitudeSquared3(m_reflectance)) < (math::real)1E-6) return; // Early exit if the diffuse color is black
	if (math::getScalar(math::magnitudeSquared3(incidentRay.getWeight())) < (math::real)1E-4) return;

	math::Vector normal = intersectionPoint.m_vertexNormal;
	math::real d = math::getScalar(math::dot(incidentRay.getDirection(), normal));
	if (d > 0) {
		normal = math::negate(normal);
	}

	rays->initializeRays(1);

	// Generate basis vectors
	math::Vector u = math::constants::YAxis;
	math::Vector v;
	math::Vector w = normal;
	if (abs(math::getX(normal)) < 0.1f) {
		u = math::constants::XAxis;
	}
	u = math::normalize(math::cross(u, normal));
	v = math::cross(normal, u);

	LightRay &ray = rays->getRays()[0];
	math::Vector weight;

	math::Vector incident = math::negate(incidentRay.getDirection());

	// Transform incident ray
	math::Vector t_dir = math::loadVector(
		math::getScalar(math::dot(incident, u)),
		math::getScalar(math::dot(incident, v)),
		math::getScalar(math::dot(incident, normal)));

	math::Vector outgoing_t;
	math::real pdf;
	
    const BSDF *bsdf = m_bsdf;

	math::Vector reflectance = bsdf->sampleF(&intersectionPoint, t_dir, &outgoing_t, &pdf, stackAllocator);

	math::Vector outgoing = math::add(
		math::mul(u, math::loadScalar(math::getX(outgoing_t))),
		math::mul(v, math::loadScalar(math::getY(outgoing_t)))
	);
	outgoing = math::add(
		outgoing,
		math::mul(math::loadScalar(math::getZ(outgoing_t)), normal)
	);

	if (pdf > 0) {
		weight = math::mul(reflectance, math::abs(math::dot(outgoing, normal)));
		weight = math::div(weight, math::loadScalar(pdf));
	} 
	else {
		outgoing = math::loadVector(1, 0, 0);
		weight = math::constants::Zero;
	}

	// Initialize the outgoing ray
	ray.setDirection(outgoing);
	ray.setWeight(weight);
	ray.setIntensity(math::constants::Zero);

	if (math::getScalar(math::dot(outgoing, normal)) >= 0) {
		ray.setSource(intersectionPoint.m_position);
	}
	else if (pdf > 0) { 
		// The light ray is undergoing transmission
		// NOTE: the pdf is checked so that the ray is not repositioned unnecessarily in degenerate cases
		ray.setSource(math::add(intersectionPoint.m_position, math::mul(incidentRay.getDirection(), math::loadScalar((math::real)1E-2))));
	}
}
