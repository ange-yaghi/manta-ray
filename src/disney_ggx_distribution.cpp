#include "../include/disney_ggx_distribution.h"

#include "../include/ggx_distribution.h"

manta::DisneyGgxDistribution::DisneyGgxDistribution() {
    m_roughness = (math::real)0.0;

    m_roughnessNode = nullptr;
    m_useNodes = true;
}

manta::DisneyGgxDistribution::~DisneyGgxDistribution() {
    /* void */
}

manta::math::real manta::DisneyGgxDistribution::getRoughness(
    const IntersectionPoint *surfaceInteraction) 
{
    if (m_roughnessNode == nullptr || !m_useNodes) return m_roughness;

    const intersection_id id = surfaceInteraction->m_id;
    const int threadId = surfaceInteraction->m_threadId;

    const DisneyGgxMemory *memory = m_cache.cacheGet(id, threadId);

    if (memory == nullptr) {
        // There was a cache miss
        DisneyGgxMemory *newMemory = m_cache.cachePut(id, threadId);
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

manta::math::real manta::DisneyGgxDistribution::getAlpha(const IntersectionPoint *surfaceInteraction) {
    math::real roughness = getRoughness(surfaceInteraction);
    return roughness * roughness;
}

manta::math::Vector manta::DisneyGgxDistribution::generateMicrosurfaceNormal(
    const IntersectionPoint *surfaceInteraction) 
{
    math::real alpha = getAlpha(surfaceInteraction);
    return GgxDistribution::generateMicrosurfaceNormal(alpha);
}

manta::math::real manta::DisneyGgxDistribution::calculateDistribution(
    const math::Vector &m, const IntersectionPoint *surfaceInteraction) 
{
    math::real roughness = getRoughness(surfaceInteraction);
    math::real alpha = roughness * roughness;

    const intersection_id id = surfaceInteraction->m_id;
    const int threadId = surfaceInteraction->m_threadId;

    const DisneyGgxMemory *memory = m_cache.cacheGet(id, threadId);
    if (memory == nullptr || !memory->calculatedDistribution) {
        DisneyGgxMemory *newMemory = m_cache.cachePut(id, threadId);
        newMemory->distribution = GgxDistribution::calculateDistribution(m, alpha);
        newMemory->roughness = roughness;
        newMemory->calculatedDistribution = true;

        memory = newMemory;
    }

    return memory->distribution;
}

manta::math::real manta::DisneyGgxDistribution::calculateG1(const math::Vector &v, 
    const math::Vector &m, const IntersectionPoint *surfaceInteraction) 
{
    math::real roughness = getRoughness(surfaceInteraction);
    math::real alpha_g = ((math::real)0.5 + roughness / 2);
    alpha_g = alpha_g * alpha_g;

    return GgxDistribution::calculateG1(v, m, alpha_g);
}

void manta::DisneyGgxDistribution::registerInputs() {
    registerInput(&m_roughnessNode, "roughness");
}

void manta::DisneyGgxDistribution::_evaluate() {
    m_output.setReference(this);
}

piranha::Node *manta::DisneyGgxDistribution::_optimize() {
    m_useNodes = true;

    if (ENABLE_OPTIMIZATION) {
        bool isConstantWidth = m_roughnessNode->getParentNode()->hasFlag(piranha::Node::META_CONSTANT);

        if (isConstantWidth) {
            math::Vector constantWidth;

            VectorNodeOutput *widthNode = static_cast<VectorNodeOutput *>(m_roughnessNode);
            widthNode->sample(nullptr, (void *)&constantWidth);

            m_roughness = math::getScalar(constantWidth);
            m_useNodes = false;
        }
    }

    return this;
}
