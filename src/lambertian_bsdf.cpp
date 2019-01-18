#include "..\include\lambertian_bsdf.h"

manta::LambertianBSDF::LambertianBSDF() {
}

manta::LambertianBSDF::~LambertianBSDF() {
}

manta::math::Vector manta::LambertianBSDF::generateMicrosurfaceNormal(const math::Vector &normal, const math::Vector &incident, const math::Vector &u, const math::Vector &v) const {
	math::real r1 = math::uniformRandom(math::constants::TWO_PI);
	math::real r2 = math::uniformRandom();
	math::real r2s = (math::real)sqrt(r2);

	math::Vector direction = math::add(
		math::mul(u, math::loadScalar(cos(r1) * r2s)),
		math::mul(v, math::loadScalar(sin(r1) * r2s)));
	direction = math::add(
		direction,
		math::mul(normal, math::loadScalar(sqrt(1 - r2))));
	
	math::Vector microfacetNormal = math::inverseReflect(direction, incident);

	return microfacetNormal;
}

manta::math::real manta::LambertianBSDF::generateWeight(const math::Vector &n, const math::Vector &i, const math::Vector &m, const math::Vector &o) const {
	return math::real(1.0);
}

manta::math::real manta::LambertianBSDF::bidirectionalShadowMasking(const math::Vector & normal, const math::Vector & incident, const math::Vector & o, const math::Vector & m) const {
	return (math::real)1.0;
}

manta::math::real manta::LambertianBSDF::g1(const math::Vector &n, const math::Vector &i, const math::Vector &v, const math::Vector &m) const {
	return (math::real)0.0 /* Not supported */;
}
