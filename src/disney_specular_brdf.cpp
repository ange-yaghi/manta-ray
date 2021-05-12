#include "../include/disney_specular_brdf.h"

#include "../include/microfacet_distribution.h"
#include "../include/vector_node_output.h"

manta::DisneySpecularBRDF::DisneySpecularBRDF() {
    m_baseColor.setDefault(math::constants::One);
    m_roughness.setDefault(math::constants::One);
    m_specular.setDefault(math::constants::One);
    m_power.setDefault(math::constants::One);

    m_distributionNode = nullptr;
    m_distribution = nullptr;
}

manta::DisneySpecularBRDF::~DisneySpecularBRDF() {
    /* void */
}

manta::math::Vector manta::DisneySpecularBRDF::sampleF(
    const IntersectionPoint *surfaceInteraction,
    const math::Vector2 &u,
    const math::Vector &i,
    math::Vector *o,
    math::real *pdf,
    RayFlags *flags,
    StackAllocator *stackAllocator)
{
    const math::Vector m = m_distribution->generateMicrosurfaceNormal(surfaceInteraction, u);
    const math::Vector ri = math::reflect(i, m);
    const math::real o_dot_m = math::getScalar(math::dot(ri, m));

    *o = ri;

    const math::real cosThetaO = math::getZ(*o);
    const math::real cosThetaI = math::getZ(i);

    if (!sameHemisphere(*o, i)) {
        *pdf = (math::real)0.0;
        return math::constants::Zero;
    }

    if (!m_distribution->isDelta(surfaceInteraction)) {
        *pdf = m_distribution->calculatePDF(m, surfaceInteraction) / ::abs(4 * o_dot_m);
        *flags = RayFlag::Reflection;
    }
    else {
        *pdf = (math::real)1.0;
        *flags = RayFlag::Reflection | RayFlag::Delta;
    }

    return DisneySpecularBRDF::f(surfaceInteraction, i, *o, stackAllocator);
}

manta::math::Vector manta::DisneySpecularBRDF::f(
    const IntersectionPoint *surfaceInteraction,
    const math::Vector &i,
    const math::Vector &o,
    StackAllocator *stackAllocator)
{
    const math::Vector wh = math::normalize(math::add(i, o));
    const math::real o_dot_wh = math::getScalar(math::dot(wh, o));

    const math::real cosThetaO = math::getZ(o);
    const math::real cosThetaI = math::getZ(i);

    if (o_dot_wh <= MIN_EPSILON ||
        cosThetaO <= MIN_EPSILON ||
        cosThetaI <= MIN_EPSILON)
    {
        return math::constants::Zero;
    }

    math::Vector reflectivity = math::constants::One;
    if (!m_distribution->isDelta(surfaceInteraction)) {
        reflectivity = math::loadScalar(
            m_distribution->calculateDistribution(wh, surfaceInteraction) *
            m_distribution->bidirectionalShadowMasking(i, o, wh, surfaceInteraction) /
            (4 * cosThetaI * cosThetaO));
    }
    else {
        reflectivity = math::loadScalar(1 / cosThetaO);
    }

    // Apply power attenuation
    const math::Vector power = m_power.sample(surfaceInteraction);
    reflectivity = math::mul(reflectivity, power);

    // Calculate Schlick Fresnel approximation
    const math::Vector F0 = m_specular.sample(surfaceInteraction);

    auto pow5 = [](math::real v) { return (v * v) * (v * v) * v; };

    const math::Vector s = math::loadScalar(pow5(1 - o_dot_wh));
    const math::Vector F = math::add(
        F0,
        math::mul(
            math::sub(math::constants::One, F0),
            s
        ));
    
    const math::Vector baseColor = m_baseColor.sample(surfaceInteraction);
    return math::mul(
        math::mul(F, reflectivity),
        baseColor);
}

manta::math::real manta::DisneySpecularBRDF::pdf(
    const IntersectionPoint *surfaceInteraction,
    const math::Vector &i,
    const math::Vector &o)
{
    if (m_distribution->isDelta(surfaceInteraction)) {
        return 0;
    }

    const math::Vector wh = math::normalize(math::add(i, o));
    const math::real o_dot_wh = math::getScalar(math::dot(wh, o));

    if (!sameHemisphere(i, o)) {
        return 0;
    }

    const math::real pdf = m_distribution->calculatePDF(wh, surfaceInteraction) / ::abs(4 * o_dot_wh);
    return pdf;
}

manta::math::Vector manta::DisneySpecularBRDF::remapSpecular(const math::Vector &specular) {
    constexpr math::Vector MaxIncidentResponse = { { (math::real)0.08, (math::real)0.08, (math::real)0.08, (math::real)0.08 } };
    return math::mul(specular, MaxIncidentResponse);
}

void manta::DisneySpecularBRDF::_evaluate() {
    BXDF::_evaluate();

    m_distribution = getObject<MicrofacetDistribution>(m_distributionNode);
}

piranha::Node *manta::DisneySpecularBRDF::_optimize(piranha::NodeAllocator *nodeAllocator) {
    m_baseColor.optimize();
    m_roughness.optimize();
    m_specular.optimize();
    m_power.optimize();

    return this;
}

void manta::DisneySpecularBRDF::registerInputs() {
    BXDF::registerInputs();

    registerInput(&m_distributionNode, "distribution");
    registerInput(m_baseColor.getPortAddress(), "base_color");
    registerInput(m_roughness.getPortAddress(), "roughness");
    registerInput(m_specular.getPortAddress(), "specular");
    registerInput(m_power.getPortAddress(), "power");
}
