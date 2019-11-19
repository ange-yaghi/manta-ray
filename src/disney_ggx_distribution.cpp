#include "../include/disney_ggx_distribution.h"

#include "../include/ggx_distribution.h"

manta::DisneyGgxDistribution::DisneyGgxDistribution() {
    m_roughness.setDefault(math::constants::One);
}

manta::DisneyGgxDistribution::~DisneyGgxDistribution() {
    /* void */
}

manta::math::real manta::DisneyGgxDistribution::getRoughness(
    const IntersectionPoint *surfaceInteraction) 
{
    return math::getScalar(m_roughness.sample(surfaceInteraction));
}

manta::math::real manta::DisneyGgxDistribution::getAlpha(const IntersectionPoint *surfaceInteraction) {
    math::real roughness = getRoughness(surfaceInteraction);
    return roughness * roughness;
}

manta::math::Vector manta::DisneyGgxDistribution::generateMicrosurfaceNormal(
    const IntersectionPoint *surfaceInteraction, const math::Vector2 &u)
{
    math::real alpha = getAlpha(surfaceInteraction);
    return GgxDistribution::generateMicrosurfaceNormal(alpha, u);
}

manta::math::real manta::DisneyGgxDistribution::calculateDistribution(
    const math::Vector &m, const IntersectionPoint *surfaceInteraction) 
{
    math::real roughness = getRoughness(surfaceInteraction);
    math::real alpha = roughness * roughness;

    const intersection_id id = surfaceInteraction->m_id;
    const int threadId = surfaceInteraction->m_threadId;

    const math::real *memory = m_distribution.cacheGet(id, threadId);
    if (memory == nullptr) {
        math::real *newMemory = m_distribution.cachePut(id, threadId);
        *newMemory = GgxDistribution::calculateDistribution(m, alpha);

        memory = newMemory;
    }

    return *memory;
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
    registerInput(m_roughness.getPortAddress(), "roughness");
}

void manta::DisneyGgxDistribution::_evaluate() {
    setOutput(this);
}

piranha::Node *manta::DisneyGgxDistribution::_optimize() {
    if (ENABLE_OPTIMIZATION) {
        m_roughness.optimize();
    }

    return this;
}
