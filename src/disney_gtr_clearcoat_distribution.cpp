#include "../include/disney_gtr_clearcoat_distribution.h"

#include "../include/vector_node_output.h"
#include "../include/ggx_distribution.h"

manta::DisneyGtrClearcoatDistribution::DisneyGtrClearcoatDistribution() {
    m_roughnessNode = nullptr;
    m_useNodes = true;
}

manta::DisneyGtrClearcoatDistribution::~DisneyGtrClearcoatDistribution() {
    /* void */
}

manta::math::real manta::DisneyGtrClearcoatDistribution::getRoughness(
    const IntersectionPoint *surfaceInteraction) 
{
    if (m_roughnessNode == nullptr || !m_useNodes) return m_roughness;

    const intersection_id id = surfaceInteraction->m_id;
    const int threadId = surfaceInteraction->m_threadId;

    const DisneyGtrClearcoatMemory *memory = m_cache.cacheGet(id, threadId);

    if (memory == nullptr) {
        // There was a cache miss
        DisneyGtrClearcoatMemory *newMemory = m_cache.cachePut(id, threadId);
        math::Vector sampledRoughness = math::constants::One;

        // Sample the width input and save it in the state container
        VectorNodeOutput *roughnessNode = static_cast<VectorNodeOutput *>(m_roughnessNode);
        roughnessNode->sample(surfaceInteraction, (void *)&sampledRoughness);

        math::real roughness = math::getScalar(sampledRoughness);
        roughness = (roughness > (math::real)1.0) ? (math::real)1.0 : roughness;
        roughness = (roughness < (math::real)0.0) ? (math::real)0.0 : roughness;

        newMemory->roughness = roughness;

        memory = newMemory;
    }

    return memory->roughness;
}

manta::math::real manta::DisneyGtrClearcoatDistribution::getAlpha(
    const IntersectionPoint *surfaceInteraction)
{
    math::real roughness = getRoughness(surfaceInteraction);
    return roughness * roughness;
}

manta::math::Vector manta::DisneyGtrClearcoatDistribution::generateMicrosurfaceNormal(
    const IntersectionPoint *surfaceInteraction)
{
    math::real alpha = getAlpha(surfaceInteraction);
    math::real alpha_2 = alpha * alpha;

    math::real r1 = math::uniformRandom();
    math::real r2 = math::uniformRandom();

    math::real rho_m = math::constants::TWO_PI * r2;
    math::real cos_theta_m_2 = (alpha_2 < 1)
        ? (1 - pow(alpha_2, 1 - r1)) / (1 - alpha_2)
        : r1;

    math::real sin_theta_m_2 = 1 - cos_theta_m_2;

    math::real cos_theta_m = ::sqrt(cos_theta_m_2);
    math::real sin_theta_m = ::sqrt(sin_theta_m_2);

    math::Vector t1 = math::loadVector(sin_theta_m, sin_theta_m, cos_theta_m);
    math::Vector t2 = math::loadVector(cos(rho_m), sin(rho_m), (math::real)1.0);

    return math::mul(t1, t2);
}

manta::math::real manta::DisneyGtrClearcoatDistribution::calculateDistribution(
    const math::Vector &m, const IntersectionPoint *surfaceInteraction)
{
    math::real alpha = getAlpha(surfaceInteraction);

    math::real cos_theta_m = math::getZ(m);

    if (cos_theta_m <= 0) return (math::real)0.0;

    math::real alpha_2 = alpha * alpha;
    math::real cos_theta_m_2 = cos_theta_m * cos_theta_m;

    math::real d_gtr;
    if (alpha_2 > 0) {
        if (alpha_2 == 1) return math::constants::INV_PI;

        math::real log_alpha_2 = ::log(alpha_2);

        d_gtr = (alpha_2 - 1);
        d_gtr /= (1 + (alpha_2 - 1) * cos_theta_m_2) * math::constants::PI * log_alpha_2;

        return d_gtr;
    }
    else return 0; // Technically a delta distribution
}

manta::math::real manta::DisneyGtrClearcoatDistribution::calculateG1(
    const math::Vector &v, const math::Vector &m, const IntersectionPoint *surfaceInteraction)
{
    constexpr math::real FIXED_ALPHA_G = (math::real)0.25;
    return GgxDistribution::calculateG1(v, m, FIXED_ALPHA_G);
}

void manta::DisneyGtrClearcoatDistribution::registerInputs() {
    registerInput(&m_roughnessNode, "roughness");
}

void manta::DisneyGtrClearcoatDistribution::_evaluate() {
    m_output.setReference(this);
}

piranha::Node *manta::DisneyGtrClearcoatDistribution::_optimize() {
    m_useNodes = true;

    if (ENABLE_OPTIMIZATION) {
        bool isConstantWidth = m_roughnessNode->getParentNode()->hasFlag(piranha::Node::META_CONSTANT);

        if (isConstantWidth) {
            math::Vector constantRoughness;

            VectorNodeOutput *roughnessNode = static_cast<VectorNodeOutput *>(m_roughnessNode);
            roughnessNode->sample(nullptr, (void *)&constantRoughness);

            m_roughness = math::getScalar(constantRoughness);
            m_useNodes = false;
        }
    }

    return this;
}