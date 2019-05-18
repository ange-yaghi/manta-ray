#include <perfect_specular_reflection_bsdf.h>

manta::PerfectSpecularReflectionBSDF::PerfectSpecularReflectionBSDF() {
	/* void */
}

manta::PerfectSpecularReflectionBSDF::~PerfectSpecularReflectionBSDF() {
	/* void */
}

manta::math::Vector manta::PerfectSpecularReflectionBSDF::sampleF(const IntersectionPoint *surfaceInteraction, const math::Vector &i, math::Vector *o, math::real *pdf, StackAllocator *stackAllocator) const {
	constexpr math::Vector reflect = { (math::real)-1.0, (math::real)-1.0, (math::real)1.0, (math::real)1.0 };

	*o = math::mul(i, reflect);
	*pdf = (math::real)1.0;

	return math::constants::One;
}
