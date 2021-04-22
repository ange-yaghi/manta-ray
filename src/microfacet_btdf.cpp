#include "../include/microfacet_btdf.h"

#include "../include/microfacet_distribution.h"

manta::MicrofacetBTDF::MicrofacetBTDF() {
    m_distribution = nullptr;
    m_mediaInterface = nullptr;

    m_distributionInput = nullptr;
    m_mediaInterfaceInput = nullptr;
}

manta::MicrofacetBTDF::~MicrofacetBTDF() {
    /* void */
}

manta::math::Vector manta::MicrofacetBTDF::sampleF(
    const IntersectionPoint *surfaceInteraction,
    const math::Vector2 &u, 
    const math::Vector &i,
    math::Vector *o,
    math::real *pdf,
    RayFlags *flags,
    StackAllocator *stackAllocator)
{
    const math::real ior = m_mediaInterface->ior(surfaceInteraction->m_direction);
    const math::Vector m = m_distribution->generateMicrosurfaceNormal(surfaceInteraction, u);
    math::Vector rt;

    if (!refract(i, m, ior, &rt)) {
        *pdf = (math::real)0.0;
        return math::constants::Zero;
    }

    if ((math::getZ(rt) > 0) == (math::getZ(i) > 0)) {
        *pdf = (math::real)0.0;
        return math::constants::Zero;
    }

    const math::real o_dot_m = math::getScalar(math::dot(rt, m));
    const math::real i_dot_m = math::getScalar(math::dot(i, m));

    if (i_dot_m <= (math::real)0.0) {
        *pdf = (math::real)0.0;
        return math::constants::Zero;
    }

    *o = rt;
    *pdf = this->pdf(surfaceInteraction, i, *o);
    *flags = RayFlag::Transmission;
    return f(surfaceInteraction, i, *o, stackAllocator);
}

manta::math::Vector manta::MicrofacetBTDF::f(
    const IntersectionPoint *surfaceInteraction,
    const math::Vector &i,
    const math::Vector &o,
    StackAllocator *stackAllocator)
{
    if (sameHemisphere(i, o)) {
        return math::constants::Zero;
    }

    const math::real ior = m_mediaInterface->ior(surfaceInteraction->m_direction);
    math::Vector m = math::normalize(math::add(o, math::mul(math::loadScalar(ior), i)));
    if (!sameHemisphere(m, i)) {
        m = math::negate(m);
    }

    const math::real o_dot_m = math::getScalar(math::dot(o, m));
    const math::real i_dot_m = math::getScalar(math::dot(i, m));

    // Calculate transmitance
    const math::real cosThetaI = ::abs(math::getZ(i));
    const math::real costhetaO = ::abs(math::getZ(o));

    const math::real F = (math::real)m_mediaInterface->fresnelTerm(std::abs(i_dot_m), surfaceInteraction->m_direction);
    const math::real Ft_num =
        ior * ior *
        m_distribution->calculateDistribution(m, surfaceInteraction) *
        m_distribution->bidirectionalShadowMasking(i, o, m, surfaceInteraction) * (1 - F) *
        ::abs(o_dot_m * i_dot_m);

    math::real Ft_div = (i_dot_m + ior * ::abs(o_dot_m));
    Ft_div *= Ft_div;
    Ft_div *= (costhetaO * cosThetaI);

    return math::mul(
        math::loadScalar(Ft_num / Ft_div),
        m_transmittance.sample(surfaceInteraction));
}

manta::math::real manta::MicrofacetBTDF::pdf(
    const IntersectionPoint *surfaceInteraction,
    const math::Vector &i,
    const math::Vector &o)
{
    if (sameHemisphere(i, o)) {
        return 0;
    }

    const math::real ior = m_mediaInterface->ior(surfaceInteraction->m_direction);
    math::Vector m = math::normalize(math::add(o, math::mul(math::loadScalar(ior), i)));
    if (!sameHemisphere(m, i)) {
        m = math::negate(m);
    }

    const math::real o_dot_m = math::getScalar(math::dot(o, m));
    const math::real i_dot_m = math::getScalar(math::dot(i, m));

    const math::real no = m_mediaInterface->no(surfaceInteraction->m_direction);
    const math::real ni = m_mediaInterface->ni(surfaceInteraction->m_direction);
    const math::real jacobian_div = (ni * ::abs(i_dot_m) + no * ::abs(o_dot_m));
    math::real jacobian = no * no * ::abs(o_dot_m);
    jacobian /= (jacobian_div * jacobian_div);

    return m_distribution->calculatePDF(m, surfaceInteraction) * jacobian;
}

void manta::MicrofacetBTDF::_evaluate() {
    BXDF::_evaluate();
    m_distribution = getObject<MicrofacetDistribution>(m_distributionInput);
    m_mediaInterface = getObject<MediaInterface>(m_mediaInterfaceInput);
}

piranha::Node *manta::MicrofacetBTDF::_optimize(piranha::NodeAllocator *nodeAllocator) {
    m_transmittance.optimize();

    return this;
}

void manta::MicrofacetBTDF::registerInputs() {
    BXDF::registerInputs();

    registerInput(&m_distributionInput, "distribution");
    registerInput(&m_mediaInterfaceInput, "media_interface");
    registerInput(m_transmittance.getPortAddress(), "transmittance");
}
