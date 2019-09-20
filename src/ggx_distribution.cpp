#include "../include/ggx_distribution.h"

#include "../include/vector_node_output.h"

#include <assert.h>

manta::GgxDistribution::GgxDistribution() {
    m_width = (math::real)1.0;
    m_minMapWidth = (math::real)0.0;
    m_widthNode = nullptr;
    m_useNodes = false;
}

manta::GgxDistribution::~GgxDistribution() {
    /* void */
}

manta::math::real manta::GgxDistribution::getWidth(const IntersectionPoint *surfaceInteraction) {
    if (!m_useNodes) return m_width;
    
    const intersection_id id = surfaceInteraction->m_id;
    const int threadId = surfaceInteraction->m_threadId;

    const GgxMemory *memory = m_cache.cacheGet(id, threadId);

    if (memory == nullptr) {
        // There was a cache miss
        GgxMemory *newMemory = m_cache.cachePut(id, threadId);
        math::Vector sampledWidth = math::constants::One;

        // Sample the width input and save it in the state container
        VectorNodeOutput *powerNode = static_cast<VectorNodeOutput *>(m_widthNode);
        powerNode->sample(surfaceInteraction, (void *)&sampledWidth);

        math::real width = math::getScalar(sampledWidth);
        width = (width > (math::real)1.0) ? (math::real)1.0 : width;
        width = (width < (math::real)0.0) ? (math::real)0.0 : width;

        newMemory->width = width; // *(m_width - m_minMapWidth) + m_minMapWidth;

        memory = newMemory;
    }

    return memory->width;
}

manta::math::Vector manta::GgxDistribution::generateMicrosurfaceNormal(
    const IntersectionPoint *surfaceInteraction) 
{
    math::real width = getWidth(surfaceInteraction);

    math::real r1 = math::uniformRandom();
    math::real r2 = math::uniformRandom();

    math::real rho_m = math::constants::TWO_PI * r2;
    math::real theta_m = ::atan(width * ::sqrt(r1) / ::sqrt(1 - r1));

    math::real cos_theta_m = ::cos(theta_m);
    math::real sin_theta_m = ::sin(theta_m);

    math::Vector t1 = math::loadVector(sin_theta_m, sin_theta_m, cos_theta_m);
    math::Vector t2 = math::loadVector(cos(rho_m), sin(rho_m), (math::real)1.0);

    return math::mul(t1, t2);
}

manta::math::real manta::GgxDistribution::calculateDistribution(
    const math::Vector &m, const IntersectionPoint *surfaceInteraction) 
{
    math::real width = getWidth(surfaceInteraction);

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
    const math::Vector &m, const IntersectionPoint *surfaceInteraction) 
{
    math::real width = getWidth(surfaceInteraction);

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
    registerInput(&m_widthNode, "width");
}

void manta::GgxDistribution::_evaluate() {
    m_output.setReference(this);
}

piranha::Node *manta::GgxDistribution::_optimize() {
    m_useNodes = true;

    if (ENABLE_OPTIMIZATION) {
        bool isConstantWidth = m_widthNode->getParentNode()->hasFlag(piranha::Node::META_CONSTANT);

        if (isConstantWidth) {
            math::Vector constantWidth;

            VectorNodeOutput *widthNode = static_cast<VectorNodeOutput *>(m_widthNode);
            widthNode->sample(nullptr, (void *)&constantWidth);

            m_width = math::getScalar(constantWidth);
            m_useNodes = false;
        }
    }

    return this;
}
