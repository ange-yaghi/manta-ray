#include "../include/perfect_specular_reflection_brdf.h"

manta::PerfectSpecularReflectionBRDF::PerfectSpecularReflectionBRDF() {
    /* void */
}

manta::PerfectSpecularReflectionBRDF::~PerfectSpecularReflectionBRDF() {
    /* void */
}

manta::math::Vector manta::PerfectSpecularReflectionBRDF::sampleF(
    const IntersectionPoint *surfaceInteraction, const math::Vector &i, math::Vector *o, 
    math::real *pdf, StackAllocator *stackAllocator) const 
{
    constexpr math::Vector reflect = { (math::real)-1.0, (math::real)-1.0, (math::real)1.0, (math::real)1.0 };

    *o = math::mul(i, reflect);
    *pdf = (math::real)1.0;

    return math::constants::One;
}

manta::math::Vector manta::PerfectSpecularReflectionBRDF::f(const IntersectionPoint *surfaceInteraction, 
    const math::Vector &i, const math::Vector &o, StackAllocator *stackAllocator) const 
{
    return math::constants::Zero;
}
