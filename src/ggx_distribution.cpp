#include "../include/ggx_distribution.h"

#include "../include/vector_node_output.h"

#include <assert.h>

manta::GgxDistribution::GgxDistribution() {
    /* void */
}

manta::GgxDistribution::~GgxDistribution() {
    /* void */
}

manta::math::real manta::GgxDistribution::getWidth(const IntersectionPoint *surfaceInteraction) {
    return math::getScalar(m_width.sample(surfaceInteraction));
}

manta::math::Vector manta::GgxDistribution::generateMicrosurfaceNormal(
    const IntersectionPoint *surfaceInteraction,
    const math::Vector2 &u)
{
    if (isDeltaInternal(surfaceInteraction)) return SurfaceAlignedMicrofacetNormal;

    const math::real width = getWidth(surfaceInteraction);
    return generateMicrosurfaceNormal(width, u);
}

manta::math::real manta::GgxDistribution::calculateDistribution(
    const math::Vector &m,
    const IntersectionPoint *surfaceInteraction) 
{
    const intersection_id id = surfaceInteraction->m_id;
    const int threadId = surfaceInteraction->m_threadId;

    const math::real *cachedDistribution = m_distribution.cacheGet({ m, id }, threadId);
    if (cachedDistribution == nullptr) {
        const math::real width = getWidth(surfaceInteraction);
        math::real *newCachedDistribution = m_distribution.cachePut({ m, id }, threadId);
        *newCachedDistribution = calculateDistribution(m, width);

        cachedDistribution = newCachedDistribution;
    }

    return *cachedDistribution;
}

manta::math::real manta::GgxDistribution::calculateG1(
    const math::Vector &v,
    const math::Vector &m,
    const IntersectionPoint *surfaceInteraction) 
{
    const math::real width = getWidth(surfaceInteraction);
    return calculateG1(v, m, width);
}

bool manta::GgxDistribution::isDelta(const IntersectionPoint *surfaceInteraction) {
    return isDeltaInternal(surfaceInteraction);
}

manta::math::Vector manta::GgxDistribution::generateMicrosurfaceNormal(
    math::real width,
    const math::Vector2 &u) 
{
    const math::real r1 = u.x;
    const math::real r2 = u.y;

    const math::real rho_m = math::constants::TWO_PI * r2;
    const math::real theta_m = ::atan(width * ::sqrt(r1) / ::sqrt(1 - r1));

    const math::real cos_theta_m = ::cos(theta_m);
    const math::real sin_theta_m = ::sin(theta_m);

    const math::Vector t1 = math::loadVector(sin_theta_m, sin_theta_m, cos_theta_m);
    const math::Vector t2 = math::loadVector(cos(rho_m), sin(rho_m), (math::real)1.0);

    return math::normalize(math::mul(t1, t2));
}

manta::math::real manta::GgxDistribution::calculateDistribution(
    const math::Vector &m,
    math::real width) 
{
    const math::real cos_theta_m = math::getZ(m);
    if (cos_theta_m <= 0) return (math::real)0.0;

    const math::real cos2_theta_m = cos_theta_m * cos_theta_m;
    const math::real tan2_theta_m = (1 - cos2_theta_m) / cos2_theta_m;
    const math::real width2 = width * width;

    // Calculate D(m)
    const math::real s1 = (width2 + tan2_theta_m) * cos2_theta_m;
    const math::real s1_2 = s1 * s1;
    if (s1_2 == 0) return (math::real)0.0;

    const math::real d_m = width2 / (math::constants::PI * s1_2);
    return d_m;
}

manta::math::real manta::GgxDistribution::calculateG1(
    const math::Vector &v,
    const math::Vector &m,
    math::real width) 
{
    const math::real v_dot_m = math::getScalar(math::dot(v, m));
    const math::real v_dot_n = (math::getZ(v));
    if ((v_dot_m < 0) != (v_dot_n < 0)) return (math::real)0.0;

    const math::real cos_theta_v = math::getZ(v);
    if (cos_theta_v == 0) return (math::real)0.0;

    const math::real cos2_theta_v = cos_theta_v * cos_theta_v;
    const math::real tan2_theta_v = (1 - cos2_theta_v) / cos2_theta_v;

    const math::real s1 = 1 + ::sqrt(1 + width * width * tan2_theta_v);
    return (math::real)2.0 / s1;
}

void manta::GgxDistribution::registerInputs() {
    registerInput(m_width.getPortAddress(), "width");
}

void manta::GgxDistribution::_evaluate() {
    setOutput(this);
}

piranha::Node *manta::GgxDistribution::_optimize(piranha::NodeAllocator *nodeAllocator) {
    if (EnableOptimization) {
        m_width.optimize();
    }

    return this;
}
