#include "../include/disney_diffuse_brdf.h"

#include "../include/vector_node_output.h"

manta::DisneyDiffuseBRDF::DisneyDiffuseBRDF() {
    m_baseColorNode = nullptr;
    m_roughnessNode = nullptr;

    m_baseColor = math::constants::One;
    m_roughness = (math::real)1.0;
}

manta::DisneyDiffuseBRDF::~DisneyDiffuseBRDF() {
    /* void */
}

manta::math::Vector manta::DisneyDiffuseBRDF::sampleF(const IntersectionPoint *surfaceInteraction,
    const math::Vector &i, math::Vector *o, math::real *pdf, StackAllocator *stackAllocator) const {
    // Uniformly sample a hemisphere
    math::real r1 = math::uniformRandom(math::constants::TWO_PI);
    math::real r2 = math::uniformRandom();
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
    const math::Vector &i, const math::Vector &o, StackAllocator *stackAllocator) const
{
    math::Vector baseColor = m_baseColor;
    math::real roughness = m_roughness;

    if (m_baseColorNode != nullptr) {
        math::Vector sampledBaseColor;
        VectorNodeOutput *baseColorNode = static_cast<VectorNodeOutput *>(m_baseColorNode);
        baseColorNode->sample(surfaceInteraction, (void *)&sampledBaseColor);

        baseColor = math::mul(baseColor, sampledBaseColor);
    }

    if (m_roughnessNode != nullptr) {
        math::Vector sampledRoughness;
        VectorNodeOutput *roughnessNode = static_cast<VectorNodeOutput *>(m_roughnessNode);
        roughnessNode->sample(surfaceInteraction, (void *)&sampledRoughness);

        roughness = math::getScalar(sampledRoughness);
    }

    math::Vector h = math::normalize(math::add(i, o));
    math::real h_dot_i = math::getScalar(math::dot(h, i));
    math::real h_dot_i_2 = h_dot_i * h_dot_i;
    math::real f_d90 = (math::real)0.5 + 2 * h_dot_i_2 * roughness;

    math::real cos_theta_i = math::getZ(i);
    math::real cos_theta_o = math::getZ(o);

    auto pow5 = [](math::real v) { return (v * v) * (v * v) * v; };

    math::real f_d = (1 + (f_d90 - 1) * pow5(1 - cos_theta_i)) * (1 + (f_d90 - 1) * pow5(1 - cos_theta_o)) / math::constants::PI;

    return math::mul(math::loadScalar(f_d), baseColor);
}

manta::math::real manta::DisneyDiffuseBRDF::pdf(
    const IntersectionPoint *surfaceInteraction, const math::Vector &i, const math::Vector &o) const
{
    return (math::real)1.0 / math::constants::TWO_PI;
}

void manta::DisneyDiffuseBRDF::registerInputs() {
    registerInput(&m_baseColorNode, "base_color");
    registerInput(&m_roughnessNode, "roughness");
}
