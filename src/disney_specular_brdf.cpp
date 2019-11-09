#include "../include/disney_specular_brdf.h"

#include "../include/microfacet_distribution.h"
#include "../include/vector_node_output.h"

manta::DisneySpecularBRDF::DisneySpecularBRDF() {
    m_distribution = nullptr;
    m_baseColor = math::constants::One;
    m_roughness = (math::real)1.0;
    m_specular = (math::real)1.0;
    m_metallic = (math::real)1.0;
    m_power = (math::real)1.0;

    m_baseColorNode = nullptr;
    m_distributionNode = nullptr;
    m_roughnessNode = nullptr;
    m_specularNode = nullptr;
    m_metallicNode = nullptr;
}

manta::DisneySpecularBRDF::~DisneySpecularBRDF() {
    /* void */
}

manta::math::Vector manta::DisneySpecularBRDF::sampleF(const IntersectionPoint *surfaceInteraction, 
    const math::Vector &i, math::Vector *o, math::real *pdf, StackAllocator *stackAllocator) const 
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
    const math::Vector &i, const math::Vector &o, StackAllocator *stackAllocator) const
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
    math::Vector power = math::loadScalar(m_power);
    if (m_powerNode != nullptr) {
        math::Vector f;
        static_cast<VectorNodeOutput *>(m_powerNode)->sample(surfaceInteraction, (void *)&f);

        power = f;
    }

    reflectivity = math::mul(reflectivity, power);

    // Calculate Schlick Fresnel approximation
    math::Vector F0;
    if (m_specularNode != nullptr) {
        math::Vector f;
        static_cast<VectorNodeOutput *>(m_specularNode)->sample(surfaceInteraction, (void *)&f);

        F0 = f;
    }
    else F0 = math::loadScalar(m_specular);

    auto pow5 = [](math::real v) { return (v * v) * (v * v) * v; };

    math::Vector s = math::loadScalar(pow5(1 - o_dot_wh));
    math::Vector F = math::add(
        F0,
        math::mul(
            math::sub(math::constants::One, F0),
            s
        ));

    // Blend metallic and dielectric models
    math::Vector baseColor = m_baseColor;
    if (m_baseColorNode != nullptr) {
        math::Vector sampledTint;
        static_cast<VectorNodeOutput *>(m_baseColorNode)->sample(surfaceInteraction, (void *)&sampledTint);

        baseColor = math::mul(baseColor, sampledTint);
    }

    math::real metallic = m_metallic;
    if (m_metallicNode != nullptr) {
        math::Vector sampledMetallic;
        static_cast<VectorNodeOutput *>(m_metallicNode)->sample(surfaceInteraction, (void *)&metallic);

        metallic = metallic * math::getScalar(sampledMetallic);
    }

    F = math::add(
        math::mul(math::loadScalar(metallic), baseColor),
        math::mul(math::loadScalar(1 - metallic), F));
    
    return math::mul(F, reflectivity);
}

manta::math::real manta::DisneySpecularBRDF::pdf(const IntersectionPoint *surfaceInteraction, 
    const math::Vector &i, const math::Vector &o) const
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

void manta::DisneySpecularBRDF::registerInputs() {
    registerInput(&m_baseColorNode, "base_color");
    registerInput(&m_metallicNode, "metallic");
    registerInput(&m_roughnessNode, "roughness");
    registerInput(&m_specularNode, "specular");
    registerInput(&m_distributionNode, "distribution");
    registerInput(&m_powerNode, "power");
}
