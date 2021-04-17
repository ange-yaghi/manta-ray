#include "../include/microfacet_brdf.h"

#include "../include/microfacet_distribution.h"
#include "../include/vector_node_output.h"

manta::MicrofacetBRDF::MicrofacetBRDF() {
    m_distribution = nullptr;
    m_distributionInput = nullptr;
}

manta::MicrofacetBRDF::~MicrofacetBRDF() {
    /* void */
}

manta::math::Vector manta::MicrofacetBRDF::sampleF(
    const IntersectionPoint *surfaceInteraction,
    const math::Vector2 &u,
    const math::Vector &i,
    math::Vector *o,
    math::real *pdf,
    StackAllocator *stackAllocator) 
{
    const math::Vector m = m_distribution->generateMicrosurfaceNormal(surfaceInteraction, u);
    const math::Vector ri = math::reflect(i, m);
    const math::real o_dot_m = math::getScalar(math::dot(ri, m));

    *o = ri;

    const math::real cosThetaO = math::getZ(*o);
    const math::real cosThetaI = math::getZ(i);

    /*
    if (o_dot_m <= MIN_EPSILON ||
        cosThetaO <= MIN_EPSILON ||
        cosThetaI <= MIN_EPSILON) 
    {
        *pdf = (math::real)0.0;
        return math::constants::Zero;
    }*/

    if (!sameHemisphere(*o, i)) {
        *pdf = (math::real)0.0;
        return math::constants::Zero;
    }

    *pdf = m_distribution->calculatePDF(m, surfaceInteraction) / ::abs(4 * o_dot_m);

    // Calculate reflectivity
    const math::real F = (math::real)1.0; // TODO: fresnel calculation goes here
    return f(surfaceInteraction, i, *o, stackAllocator);
}

manta::math::Vector manta::MicrofacetBRDF::f(
    const IntersectionPoint *surfaceInteraction,
    const math::Vector &i,
    const math::Vector &o,
    StackAllocator *stackAllocator)
{
    math::Vector wh = math::add(i, o);

    const math::real cosThetaO = std::abs(math::getZ(o));
    const math::real cosThetaI = std::abs(math::getZ(i));

    if (cosThetaO == 0 || cosThetaI == 0) return math::constants::Zero;
    if (math::getX(wh) == 0 || math::getY(wh) == 0 || math::getZ(wh) == 0) return math::constants::Zero;

    wh = math::normalize(wh);

    const math::real o_dot_wh = math::getScalar(math::dot(wh, o));
    const math::real F = (math::real)1.0; // TODO: fresnel calculation goes here

    /*
    if (o_dot_wh <= MIN_EPSILON ||
        cosThetaO <= MIN_EPSILON ||
        cosThetaI <= MIN_EPSILON)
    {
        return math::constants::Zero;
    }*/

    const math::Vector reflectivity = math::loadScalar(
        m_distribution->calculateDistribution(wh, surfaceInteraction) *
        m_distribution->bidirectionalShadowMasking(i, o, wh, surfaceInteraction) *
        F / (4 * cosThetaI * cosThetaO));

    return math::mul(
        reflectivity,
        getReflectivity(surfaceInteraction));
}

manta::math::real manta::MicrofacetBRDF::pdf(
    const IntersectionPoint *surfaceInteraction,
    const math::Vector &i,
    const math::Vector &o) 
{
    if (!sameHemisphere(i, o)) {
        return 0;
    }

    const math::Vector wh = math::normalize(math::add(i, o));
    const math::real o_dot_wh = math::getScalar(math::dot(wh, o));

    const math::real cosThetaO = math::getZ(o);
    const math::real cosThetaI = math::getZ(i);

    /*
    if (o_dot_wh <= MIN_EPSILON ||
        cosThetaO <= MIN_EPSILON ||
        cosThetaI <= MIN_EPSILON)
    {
        return (math::real)0.0;
    }*/

    const math::real pdf = m_distribution->calculatePDF(wh, surfaceInteraction) / ::abs(4 * o_dot_wh);
    return pdf;
}

void manta::MicrofacetBRDF::setBaseReflectivity(const math::Vector &reflectivity) {
    m_reflectivity.setDefault(reflectivity);
}

manta::math::Vector manta::MicrofacetBRDF::getReflectivity(
    const IntersectionPoint *surfaceInteraction) 
{
    return m_reflectivity.sample(surfaceInteraction);
}

void manta::MicrofacetBRDF::_evaluate() {
    BXDF::_evaluate();
    m_distribution = getObject<MicrofacetDistribution>(m_distributionInput);
}

piranha::Node * manta::MicrofacetBRDF::_optimize(piranha::NodeAllocator *nodeAllocator) {
    m_reflectivity.optimize();

    return this;
}

void manta::MicrofacetBRDF::registerInputs() {
    BXDF::registerInputs();

    registerInput(&m_distributionInput, "distribution");
    registerInput(m_reflectivity.getPortAddress(), "reflectivity");
}
