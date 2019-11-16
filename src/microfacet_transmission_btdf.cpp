#include "../include/microfacet_transmission_btdf.h"

#include "../include/microfacet_distribution.h"

manta::MicrofacetTransmissionBTDF::MicrofacetTransmissionBTDF() {
    m_distribution = nullptr;
}

manta::MicrofacetTransmissionBTDF::~MicrofacetTransmissionBTDF() {
    /* void */
}

manta::math::Vector manta::MicrofacetTransmissionBTDF::sampleF(
    const IntersectionPoint *surfaceInteraction, const math::Vector2 &u, 
    const math::Vector &i, math::Vector *o,
    math::real *pdf, StackAllocator *stackAllocator) const 
{
    constexpr math::Vector reflect = { (math::real)-1.0, (math::real)-1.0, (math::real)1.0, (math::real)1.0 };

    math::real ior = m_mediaInterface->ior(surfaceInteraction->m_direction);
    math::Vector m = m_distribution->generateMicrosurfaceNormal(surfaceInteraction, u);
    math::Vector rt;

    if (!refract(i, m, ior, &rt)) {
        *pdf = (math::real)0.0;
        return math::constants::Zero;
    }

    if ((math::getZ(rt) > 0) == (math::getZ(i) > 0)) {
        *pdf = (math::real)0.0;
        return math::constants::Zero;
    }

    math::real o_dot_m = math::getScalar(math::dot(rt, m));
    math::real i_dot_m = math::getScalar(math::dot(i, m));

    *o = rt;

    if (i_dot_m <= (math::real)0.0) {
        *pdf = (math::real)0.0;
        return math::constants::Zero;
    }

    math::real no = m_mediaInterface->no(surfaceInteraction->m_direction);
    math::real ni = m_mediaInterface->ni(surfaceInteraction->m_direction);
    math::real jacobian = no * no * ::abs(o_dot_m);
    math::real jacobian_div = (ni * ::abs(i_dot_m) + no * ::abs(o_dot_m));
    jacobian /= (jacobian_div * jacobian_div);

    *pdf = m_distribution->calculatePDF(m, surfaceInteraction) * jacobian;

    // Calculate transmitance
    math::real cosThetaI = ::abs(math::getZ(i));
    math::real costhetaO = ::abs(math::getZ(rt));

    math::real F = (math::real)m_mediaInterface->fresnelTerm(i, m, surfaceInteraction->m_direction);

    math::real Ft_num = 
        ior * ior * 
        m_distribution->calculateDistribution(m, surfaceInteraction) * 
        m_distribution->bidirectionalShadowMasking(i, rt, m, surfaceInteraction) * (1 - F);
    Ft_num *= ::abs(o_dot_m * i_dot_m);

    math::real Ft_div = (i_dot_m + ior * ::abs(o_dot_m));
    Ft_div *= Ft_div;
    Ft_div *= (costhetaO * cosThetaI);

    math::Vector transmitance = math::loadScalar(Ft_num / Ft_div);

    return transmitance;
}

manta::math::Vector manta::MicrofacetTransmissionBTDF::f(const IntersectionPoint *surfaceInteraction, 
    const math::Vector &i, const math::Vector &o, StackAllocator *stackAllocator) const 
{
    return math::constants::Zero;
}

manta::math::real manta::MicrofacetTransmissionBTDF::pdf(
    const IntersectionPoint *surfaceInteraction, const math::Vector &i, const math::Vector &o) const 
{
    return math::real(0.0);
}

