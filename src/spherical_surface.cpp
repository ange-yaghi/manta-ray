#include <spherical_surface.h>

#include <intersection_point.h>
#include <light_ray.h>
#include <lens_element.h>

manta::SphericalSurface::SphericalSurface() {
	m_convex = true;
	m_input = true;
	m_ior = (math::real)0.0;
}

manta::SphericalSurface::~SphericalSurface() {

}

bool manta::SphericalSurface::transformLightRay(const LightRay *ray, LightRay *outputRay) const {
	IntersectionPoint point;
	if (m_convex) {
		m_sphere.detectIntersection(ray, &point, nullptr);
	}
	else {
		m_sphere.detectIntersection(ray, nullptr, &point);
	}

	if (!point.m_intersection) {
		return false;
	}

	math::real r;

	if (m_input) {
		r = (math::real)1.0 / m_ior;
	}
	else {
		r = m_ior / (math::real)1.0;
	}

	math::real c = -math::getScalar(math::dot(ray->getDirection(), point.m_vertexNormal));

	math::real cos2t = 1 - r * r * (1 - c * c);

	if (cos2t < (math::real)0.0) {
		outputRay->setDirection(ray->getDirection());
		outputRay->setSource(point.m_position);
		return true;
	}

	math::Vector tdir = math::mul(ray->getDirection(), math::loadScalar(r));
	tdir = math::add(
		tdir,
		math::mul(
			point.m_vertexNormal,
			math::loadScalar(c * r - (math::real)sqrt(cos2t))));
	tdir = math::normalize(tdir);

	outputRay->setSource(point.m_position);
	outputRay->setDirection(tdir);

	return true;
}
