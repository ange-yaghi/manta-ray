#include "../include/perfect_specular_reflection_brdf.h"

manta::PerfectSpecularReflectionBRDF::PerfectSpecularReflectionBRDF() {
    /* void */
}

manta::PerfectSpecularReflectionBRDF::~PerfectSpecularReflectionBRDF() {
    /* void */
}

manta::math::Vector manta::PerfectSpecularReflectionBRDF::sampleF(
    const IntersectionPoint *surfaceInteraction,
    const math::Vector2 &u,
    const math::Vector &i,
    math::Vector *o, 
    math::real *pdf,
    RayFlags *flags,
    StackAllocator *stackAllocator)
{
    constexpr math::Vector reflect = { { (math::real)-1.0, (math::real)-1.0, (math::real)1.0, (math::real)1.0 } };

    *o = math::mul(i, reflect);
    *pdf = (math::real)1.0;
    *flags = RayFlag::Delta;

    return math::constants::One;
}

manta::math::Vector manta::PerfectSpecularReflectionBRDF::f(
    const IntersectionPoint *surfaceInteraction,
    const math::Vector &i,
    const math::Vector &o,
    StackAllocator *stackAllocator)
{
    return math::constants::Zero;
}

manta::math::real manta::PerfectSpecularReflectionBRDF::pdf(
    const IntersectionPoint *surfaceInteraction,
    const math::Vector &i,
    const math::Vector &o)
{
    return 0;
}
