#include "../include/microfacet_brdf.h"

#include "../include/microfacet_distribution.h"
#include "../include/vector_node_output.h"

manta::MicrofacetBRDF::MicrofacetBRDF() {
    m_distribution = nullptr;
    m_distributionInput = nullptr;
    m_reflectivityInput = nullptr;

    m_baseReflectivity = math::constants::One;
}

manta::MicrofacetBRDF::~MicrofacetBRDF() {
    /* void */
}

manta::math::Vector manta::MicrofacetBRDF::sampleF(
        const IntersectionPoint *surfaceInteraction, const math::Vector &i, 
        math::Vector *o, math::real *pdf, StackAllocator *stackAllocator) const 
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

    // Calculate reflectivity
    math::real F = (math::real)1.0; // TODO: fresnel calculation goes here

    math::Vector reflectivity = math::loadScalar(
        m_distribution->calculateDistribution(m, surfaceInteraction) * 
        m_distribution->bidirectionalShadowMasking(i, *o, m, surfaceInteraction) * 
        F / (4 * cosThetaI * cosThetaO));

    return math::mul(
        reflectivity,
        getReflectivity(surfaceInteraction));
}

manta::math::Vector manta::MicrofacetBRDF::f(const IntersectionPoint *surfaceInteraction,
    const math::Vector &i, const math::Vector &o, StackAllocator *stackAllocator) const 
{
    math::Vector wh = math::normalize(math::add(i, o));
    math::real o_dot_wh = math::getScalar(math::dot(wh, o));
    math::real F = (math::real)1.0; // TODO: fresnel calculation goes here

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
        m_distribution->bidirectionalShadowMasking(i, o, wh, surfaceInteraction) *
        F / (4 * cosThetaI * cosThetaO));

    return math::mul(
        reflectivity,
        getReflectivity(surfaceInteraction));
}

manta::math::real manta::MicrofacetBRDF::pdf(
    const IntersectionPoint *surfaceInteraction, const math::Vector &i, const math::Vector &o) const 
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

void manta::MicrofacetBRDF::setBaseReflectivity(const math::Vector &reflectivity) {
    m_baseReflectivity = reflectivity;
}

manta::math::Vector manta::MicrofacetBRDF::getReflectivity(
    const IntersectionPoint *surfaceInteraction) const 
{
    if (m_reflectivityInput == nullptr) return m_baseReflectivity;
    else {
        math::Vector f;
        static_cast<VectorNodeOutput *>(m_reflectivityInput)->sample(surfaceInteraction, (void *)&f);
        
        return math::mul(f, m_baseReflectivity);
    }
}

void manta::MicrofacetBRDF::_evaluate() {
    BXDF::_evaluate();

    ObjectReferenceNodeOutput<MicrofacetDistribution> *distInput = 
        static_cast<ObjectReferenceNodeOutput<MicrofacetDistribution> *>(m_distributionInput);
    m_distribution = distInput->getReference();
}

void manta::MicrofacetBRDF::registerInputs() {
    registerInput(&m_distributionInput, "distribution");
    registerInput(&m_reflectivityInput, "reflectivity");
}
