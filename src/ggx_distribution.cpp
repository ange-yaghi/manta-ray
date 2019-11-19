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
    const IntersectionPoint *surfaceInteraction, const math::Vector2 &u)
{
    math::real width = getWidth(surfaceInteraction);
    return generateMicrosurfaceNormal(width, u);
}

manta::math::real manta::GgxDistribution::calculateDistribution(
    const math::Vector &m, const IntersectionPoint *surfaceInteraction) 
{
    math::real width = getWidth(surfaceInteraction);

    const intersection_id id = surfaceInteraction->m_id;
    const int threadId = surfaceInteraction->m_threadId;

    const math::real *memory = m_distribution.cacheGet(id, threadId);

    if (memory == nullptr) {
        math::real *newValue = m_distribution.cachePut(id, threadId);
        *newValue = calculateDistribution(m, width);

        memory = newValue;
    }

    return *memory;
}

manta::math::real manta::GgxDistribution::calculateG1(const math::Vector &v, 
    const math::Vector &m, const IntersectionPoint *surfaceInteraction) 
{
    math::real width = getWidth(surfaceInteraction);
    return calculateG1(v, m, width);
}

manta::math::Vector manta::GgxDistribution::generateMicrosurfaceNormal(
    math::real width, const math::Vector2 &u) 
{
    math::real r1 = u.x;
    math::real r2 = u.y;

    math::real rho_m = math::constants::TWO_PI * r2;
    math::real theta_m = ::atan(width * ::sqrt(r1) / ::sqrt(1 - r1));

    math::real cos_theta_m = ::cos(theta_m);
    math::real sin_theta_m = ::sin(theta_m);

    math::Vector t1 = math::loadVector(sin_theta_m, sin_theta_m, cos_theta_m);
    math::Vector t2 = math::loadVector(cos(rho_m), sin(rho_m), (math::real)1.0);

    return math::mul(t1, t2);
}

manta::math::real manta::GgxDistribution::calculateDistribution(
    const math::Vector &m, math::real width) 
{
    math::real cos_theta_m = math::getZ(m);

    if (cos_theta_m <= 0) return (math::real)0.0;

    math::real cos2_theta_m = cos_theta_m * cos_theta_m;
    math::real tan2_theta_m = (1 - cos2_theta_m) / cos2_theta_m;
    math::real width2 = width * width;

    // Calculate D(m)
    math::real s1 = (width2 + tan2_theta_m) * cos2_theta_m;
    math::real s1_2 = s1 * s1;

    if (s1_2 == 0) return (math::real)0.0;

    math::real d_m = width2 / (math::constants::PI * s1_2);

    return d_m;
}

manta::math::real manta::GgxDistribution::calculateG1(const math::Vector &v, 
    const math::Vector &m, math::real width) 
{
    math::real v_dot_m = math::getScalar(math::dot(v, m));
    math::real v_dot_n = (math::getZ(v));
    if ((v_dot_m < 0) != (v_dot_n < 0)) return (math::real)0.0;

    math::real cos_theta_v = math::getZ(v);
    if (cos_theta_v == 0) return (math::real)0.0;

    math::real cos2_theta_v = cos_theta_v * cos_theta_v;
    math::real tan2_theta_v = (1 - cos2_theta_v) / cos2_theta_v;

    math::real s1 = 1 + ::sqrt(1 + width * width * tan2_theta_v);

    return (math::real)2.0 / s1;
}

void manta::GgxDistribution::registerInputs() {
    registerInput(m_width.getPortAddress(), "width");
}

void manta::GgxDistribution::_evaluate() {
    setOutput(this);
}

piranha::Node *manta::GgxDistribution::_optimize() {
    if (ENABLE_OPTIMIZATION) {
        m_width.optimize();
    }

    return this;
}
