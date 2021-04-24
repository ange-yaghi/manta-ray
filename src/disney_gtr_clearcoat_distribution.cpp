#include "../include/disney_gtr_clearcoat_distribution.h"

#include "../include/vector_node_output.h"
#include "../include/ggx_distribution.h"

manta::DisneyGtrClearcoatDistribution::DisneyGtrClearcoatDistribution() {
    m_roughness.setDefault(math::constants::One);
}

manta::DisneyGtrClearcoatDistribution::~DisneyGtrClearcoatDistribution() {
    /* void */
}

manta::math::real manta::DisneyGtrClearcoatDistribution::getRoughness(
    const IntersectionPoint *surfaceInteraction) 
{
    return math::getScalar(m_roughness.sample(surfaceInteraction));
}

manta::math::real manta::DisneyGtrClearcoatDistribution::getAlpha(
    const IntersectionPoint *surfaceInteraction)
{
    const math::real roughness = getRoughness(surfaceInteraction);
    return roughness * roughness;
}

manta::math::Vector manta::DisneyGtrClearcoatDistribution::generateMicrosurfaceNormal(
    const IntersectionPoint *surfaceInteraction,
    const math::Vector2 &u)
{
    if (isDeltaInternal(surfaceInteraction)) return SurfaceAlignedMicrofacetNormal;

    const math::real alpha = getAlpha(surfaceInteraction);
    const math::real alpha_2 = alpha * alpha;

    const math::real r1 = u.x;
    const math::real r2 = u.y;

    const math::real rho_m = math::constants::TWO_PI * r2;
    const math::real cos_theta_m_2 = (alpha_2 < 1)
        ? (1 - pow(alpha_2, 1 - r1)) / (1 - alpha_2)
        : r1;

    const math::real sin_theta_m_2 = 1 - cos_theta_m_2;

    const  math::real cos_theta_m = ::sqrt(cos_theta_m_2);
    const math::real sin_theta_m = ::sqrt(sin_theta_m_2);

    const math::Vector t1 = math::loadVector(sin_theta_m, sin_theta_m, cos_theta_m);
    const math::Vector t2 = math::loadVector(cos(rho_m), sin(rho_m), (math::real)1.0);

    return math::mul(t1, t2);
}

manta::math::real manta::DisneyGtrClearcoatDistribution::calculateDistribution(
    const math::Vector &m,
    const IntersectionPoint *surfaceInteraction)
{
    const intersection_id id = surfaceInteraction->m_id;
    const int threadId = surfaceInteraction->m_threadId;

    const math::real *cachedDistribution = m_distribution.cacheGet({ m, id }, threadId);
    if (cachedDistribution == nullptr) {
        math::real *newCachedDistribution = m_distribution.cachePut({ m, id }, threadId);
        *newCachedDistribution = recalculateDistribution(m, surfaceInteraction);
        cachedDistribution = newCachedDistribution;
    }

    return *cachedDistribution;
}

manta::math::real manta::DisneyGtrClearcoatDistribution::calculateG1(
    const math::Vector &v,
    const math::Vector &m,
    const IntersectionPoint *surfaceInteraction)
{
    constexpr math::real FixedAlphaG = (math::real)0.25;
    return GgxDistribution::calculateG1(v, m, FixedAlphaG);
}

bool manta::DisneyGtrClearcoatDistribution::isDelta(const IntersectionPoint *surfaceInteraction) {
    return isDeltaInternal(surfaceInteraction);
}

manta::math::real manta::DisneyGtrClearcoatDistribution::recalculateDistribution(
    const math::Vector &m,
    const IntersectionPoint *surfaceInteraction)
{
    const math::real alpha = getAlpha(surfaceInteraction);
    const math::real cos_theta_m = math::getZ(m);
    if (cos_theta_m <= 0) return (math::real)0.0;

    const math::real alpha_2 = alpha * alpha;
    const math::real cos_theta_m_2 = cos_theta_m * cos_theta_m;

    if (alpha_2 > 0) {
        if (alpha_2 == 1) return math::constants::INV_PI;

        const math::real log_alpha_2 = ::log(alpha_2);
        math::real d_gtr = (alpha_2 - 1);
        return d_gtr / ((1 + (alpha_2 - 1) * cos_theta_m_2) * math::constants::PI * log_alpha_2);
    }
    else return 0; // Technically a delta distribution
}

void manta::DisneyGtrClearcoatDistribution::registerInputs() {
    registerInput(m_roughness.getPortAddress(), "roughness");
}

void manta::DisneyGtrClearcoatDistribution::_evaluate() {
    setOutput(this);
}

piranha::Node *manta::DisneyGtrClearcoatDistribution::_optimize(piranha::NodeAllocator *nodeAllocator) {
    if (EnableOptimization) {
        m_roughness.optimize();
    }

    return this;
}
