#include <perfect_specular_reflection_bsdf.h>

manta::PerfectSpecularReflectionBSDF::PerfectSpecularReflectionBSDF() {

}

manta::PerfectSpecularReflectionBSDF::~PerfectSpecularReflectionBSDF() {

}

manta::math::Vector manta::PerfectSpecularReflectionBSDF::sampleF(const math::Vector &i, math::Vector *o, math::real *pdf) const {
	constexpr math::Vector reflect = { (math::real)-1.0, (math::real)-1.0, (math::real)1.0, (math::real)1.0 };

	*o = math::mul(i, reflect);
	*pdf = (math::real)1.0;

	return math::constants::One;
}
