#include "../include/disney_diffuse_brdf.h"

#include "../include/vector_node_output.h"

manta::DisneyDiffuseBRDF::DisneyDiffuseBRDF() {
    m_baseColor.setDefault(math::constants::One);
    m_roughness.setDefault(math::constants::One);
    m_power.setDefault(math::constants::One);
}

manta::DisneyDiffuseBRDF::~DisneyDiffuseBRDF() {
    /* void */
}

manta::math::Vector manta::DisneyDiffuseBRDF::sampleF(
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

    return f(surfaceInteraction, i, *o, stackAllocator);
}

manta::math::Vector manta::DisneyDiffuseBRDF::f(const IntersectionPoint *surfaceInteraction,
    const math::Vector &i, const math::Vector &o, StackAllocator *stackAllocator)
{
    math::Vector baseColor = m_baseColor.sample(surfaceInteraction);
    math::Vector power = m_power.sample(surfaceInteraction);
    math::real roughness = math::getScalar(m_roughness.sample(surfaceInteraction));

    math::Vector h = math::normalize(math::add(i, o));
    math::real h_dot_i = math::getScalar(math::dot(h, i));
    math::real h_dot_i_2 = h_dot_i * h_dot_i;
    math::real f_d90 = (math::real)0.5 + 2 * h_dot_i_2 * roughness;

    math::real cos_theta_i = math::getZ(i);
    math::real cos_theta_o = math::getZ(o);

    auto pow5 = [](math::real v) { return (v * v) * (v * v) * v; };

    math::real f_d = (1 + (f_d90 - 1) * pow5(1 - cos_theta_i)) * (1 + (f_d90 - 1) * pow5(1 - cos_theta_o)) / math::constants::PI;

    math::Vector f = math::mul(math::loadScalar(f_d), baseColor);
    f = math::mul(f, power);
    return f;
}

manta::math::real manta::DisneyDiffuseBRDF::pdf(
    const IntersectionPoint *surfaceInteraction, const math::Vector &i, const math::Vector &o)
{
    if (math::getZ(o) < 0) {
        return 0;
    }

    return (math::real)1.0 / math::constants::TWO_PI;
}

piranha::Node *manta::DisneyDiffuseBRDF::_optimize(piranha::NodeAllocator *nodeAllocator) {
    m_power.optimize();
    m_baseColor.optimize();
    m_roughness.optimize();

    return this;
}

void manta::DisneyDiffuseBRDF::registerInputs() {
    BXDF::registerInputs();

    registerInput(m_baseColor.getPortAddress(), "base_color");
    registerInput(m_roughness.getPortAddress(), "roughness");
    registerInput(m_power.getPortAddress(), "power");
}
