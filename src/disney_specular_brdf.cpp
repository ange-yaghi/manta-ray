#include "../include/disney_specular_brdf.h"

#include "../include/microfacet_distribution.h"
#include "../include/vector_node_output.h"

manta::DisneySpecularBRDF::DisneySpecularBRDF() {
    m_distribution = nullptr;
    m_baseColor.setDefault(math::constants::One);
    m_roughness.setDefault(math::constants::One);
    m_specular.setDefault(math::constants::One);
    m_power.setDefault(math::constants::One);

    m_distributionNode = nullptr;
}

manta::DisneySpecularBRDF::~DisneySpecularBRDF() {
    /* void */
}

manta::math::Vector manta::DisneySpecularBRDF::sampleF(const IntersectionPoint *surfaceInteraction, 
    const math::Vector &i, math::Vector *o, math::real *pdf, StackAllocator *stackAllocator) 
{
    constexpr math::Vector reflect = { (math::real)-1.0, (math::real)-1.0, (math::real)1.0, (math::real)1.0 };

    math::Vector m = m_distribution->generateMicrosurfaceNormal(surfaceInteraction);
    math::Vector ri = math::reflect(i, m);
    math::real o_dot_m = math::getScalar(math::dot(ri, m));

    *o = ri;

    math::real cosThetaO = math::getZ(*o);
    math::real cosThetaI = math::getZ(i);

    if (o_dot_m <= MIN_EPSILON ||
        cosThetaO <= MIN_EPSILON ||
        cosThetaI <= MIN_EPSILON)
    {
        *pdf = (math::real)0.0;
        return math::constants::Zero;
    }

    *pdf = m_distribution->calculatePDF(m, surfaceInteraction) / ::abs(4 * o_dot_m);

    return DisneySpecularBRDF::f(surfaceInteraction, i, *o, stackAllocator);
}

manta::math::Vector manta::DisneySpecularBRDF::f(const IntersectionPoint *surfaceInteraction, 
    const math::Vector &i, const math::Vector &o, StackAllocator *stackAllocator)
{
    math::Vector wh = math::normalize(math::add(i, o));
    math::real o_dot_wh = math::getScalar(math::dot(wh, o));

    math::real cosThetaO = math::getZ(o);
    math::real cosThetaI = math::getZ(i);

    if (o_dot_wh <= MIN_EPSILON ||
        cosThetaO <= MIN_EPSILON ||
        cosThetaI <= MIN_EPSILON)
    {
        return math::constants::Zero;
    }

    math::Vector reflectivity = math::loadScalar(
        m_distribution->calculateDistribution(wh, surfaceInteraction) *
        m_distribution->bidirectionalShadowMasking(i, o, wh, surfaceInteraction) / 
        (4 * cosThetaI * cosThetaO));

    // Apply power attenuation
    math::Vector power = m_power.sample(surfaceInteraction);

    reflectivity = math::mul(reflectivity, power);

    // Calculate Schlick Fresnel approximation
    math::Vector F0 = m_specular.sample(surfaceInteraction);

    auto pow5 = [](math::real v) { return (v * v) * (v * v) * v; };

    math::Vector s = math::loadScalar(pow5(1 - o_dot_wh));
    math::Vector F = math::add(
        F0,
        math::mul(
            math::sub(math::constants::One, F0),
            s
        ));
    
    return math::mul(F, reflectivity);
}

manta::math::real manta::DisneySpecularBRDF::pdf(const IntersectionPoint *surfaceInteraction, 
    const math::Vector &i, const math::Vector &o)
{
    math::Vector wh = math::normalize(math::add(i, o));
    math::real o_dot_wh = math::getScalar(math::dot(wh, o));

    math::real cosThetaO = math::getZ(o);
    math::real cosThetaI = math::getZ(i);

    if (o_dot_wh <= MIN_EPSILON ||
        cosThetaO <= MIN_EPSILON ||
        cosThetaI <= MIN_EPSILON)
    {
        return (math::real)0.0;
    }

    math::real pdf = m_distribution->calculatePDF(wh, surfaceInteraction) / ::abs(4 * o_dot_wh);

    return pdf;
}

manta::math::Vector manta::DisneySpecularBRDF::remapSpecular(const math::Vector &specular) {
    constexpr math::Vector MAX_INCIDENT_RESPONSE = { 
        (math::real)0.08, (math::real)0.08, (math::real)0.08, (math::real)0.08 };

    return math::mul(specular, MAX_INCIDENT_RESPONSE);
}

void manta::DisneySpecularBRDF::_evaluate() {
    BXDF::_evaluate();

    m_distribution = getObject<MicrofacetDistribution>(m_distributionNode);
}

piranha::Node *manta::DisneySpecularBRDF::_optimize() {
    m_baseColor.optimize();
    m_roughness.optimize();
    m_specular.optimize();
    m_power.optimize();

    return this;
}

void manta::DisneySpecularBRDF::registerInputs() {
    registerInput(m_baseColor.getPortAddress(), "base_color");
    registerInput(m_roughness.getPortAddress(), "roughness");
    registerInput(m_specular.getPortAddress(), "specular");
    registerInput(&m_distributionNode, "distribution");
    registerInput(m_power.getPortAddress(), "power");
}
