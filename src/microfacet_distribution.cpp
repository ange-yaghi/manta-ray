#include "../include/microfacet_distribution.h"

#include "../include/stack_allocator.h"

manta::MicrofacetDistribution::MicrofacetDistribution() {
    /* void */
}

manta::MicrofacetDistribution::~MicrofacetDistribution() {
    /* void */
}

manta::math::real manta::MicrofacetDistribution::calculatePDF(
    const math::Vector &m, const IntersectionPoint *surfaceInteraction) 
{
    math::real cos_theta_m = math::getZ(m);
    return calculateDistribution(m, surfaceInteraction) * cos_theta_m;
}

manta::math::real manta::MicrofacetDistribution::bidirectionalShadowMasking(
    const math::Vector &i, const math::Vector &o, 
    const math::Vector &m, const IntersectionPoint *surfaceInteraction) 
{
    return smithBidirectionalShadowMasking(i, o, m, surfaceInteraction);
}

manta::math::real manta::MicrofacetDistribution::smithBidirectionalShadowMasking(
    const math::Vector &i, const math::Vector &o, 
    const math::Vector &m, const IntersectionPoint *surfaceInteraction)
{
    return calculateG1(i, m, surfaceInteraction) * calculateG1(o, m, surfaceInteraction);
}
