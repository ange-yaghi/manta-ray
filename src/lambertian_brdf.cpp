#include "../include/lambertian_brdf.h"

manta::LambertianBRDF::LambertianBRDF() {
    /* void */
}

manta::LambertianBRDF::~LambertianBRDF() {
    /* void */
}

manta::math::Vector manta::LambertianBRDF::sampleF(
    const IntersectionPoint *surfaceInteraction, 
    const math::Vector2 &u,
    const math::Vector &i,
    math::Vector *o,
    math::real *pdf,
    RayFlags *flags,
    StackAllocator *stackAllocator)
{
    // Uniformly sample a hemisphere
    math::real r1 = u.x * math::constants::TWO_PI;
    math::real r2 = u.y;
    math::real r2s = (math::real)sqrt(1 - r2 * r2);

    math::Vector direction = math::loadVector(
        cos(r1) * r2s,
        sin(r1) * r2s,
        r2);

    *o = direction;
    *pdf = (math::real)1.0 / math::constants::TWO_PI;
    *flags = RayFlag::Reflection;

    return math::mul(
        m_color.sample(surfaceInteraction),
        math::loadScalar((math::real)1.0 / math::constants::PI));
}

manta::math::Vector manta::LambertianBRDF::f(
    const IntersectionPoint *surfaceInteraction,
    const math::Vector &i,
    const math::Vector &o,
    StackAllocator *stackAllocator)
{
    return math::mul(
        m_color.sample(surfaceInteraction),
        math::loadScalar((math::real)1.0 / math::constants::PI));
}

manta::math::real manta::LambertianBRDF::pdf(
    const IntersectionPoint *surfaceInteraction,
    const math::Vector &i,
    const math::Vector &o) 
{
    if (math::getZ(o) < 0) {
        return 0;
    }

    return (math::real)1.0 / math::constants::TWO_PI;
}

piranha::Node *manta::LambertianBRDF::_optimize(piranha::NodeAllocator *nodeAllocator) {
    m_color.optimize();

    return this;
}

void manta::LambertianBRDF::registerInputs() {
    BXDF::registerInputs();

    registerInput(m_color.getPortAddress(), "color");
}
