#include <simple_lambert_material.h>

#include <ray_container.h>

#include <intersection_point.h>
#include <light_ray.h>

manta::SimpleLambertMaterial::SimpleLambertMaterial() {
	m_maxDegree = 5;
}

manta::SimpleLambertMaterial::~SimpleLambertMaterial() {
}

void manta::SimpleLambertMaterial::integrateRay(LightRay *ray, const RayContainer &rays, const IntersectionPoint &intersectionPoint) const {
	math::Vector totalLight = m_emission;

	if (rays.getRayCount() > 0) {
		const LightRay &mainRay = rays.getRays()[0];
		totalLight = math::add(totalLight,
			math::mul(
				mainRay.getWeightedIntensity(),
				m_diffuseColor));
			//ray->setIntensity(math::loadVector(0.0, 1.0, 0.0));
	}

	//totalLight = math::add(math::mul(intersectionPoint->m_vertexNormal, math::constants::Half), math::loadVector(0.5, 0.5, 0.5));

	ray->setIntensity(totalLight);
}

void manta::SimpleLambertMaterial::generateRays(RayContainer *rays, const LightRay &incidentRay, const IntersectionPoint &intersectionPoint, int degree, StackAllocator *stackAllocator) const {
	if (degree > m_maxDegree) return;
	if (math::getScalar(math::magnitudeSquared3(m_diffuseColor)) < (math::real)1E-6) return; /* Early exit if the diffuse color is black */

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
	math::real weight;

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

	// Initialize the outgoing ray
	ray.setDirection(o);
	ray.setWeight(weight);
	ray.setIntensity(math::constants::Zero);
	ray.setSource(intersectionPoint.m_position);
}
