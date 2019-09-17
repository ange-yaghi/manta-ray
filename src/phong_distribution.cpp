#include "../include/phong_distribution.h"

#include "../include/vector_node_output.h"

#include <assert.h>

manta::PhongDistribution::PhongDistribution() {
    m_power = (math::real)2.0;
    m_minMapPower = (math::real)1.0;
    m_powerNode = nullptr;
}

manta::PhongDistribution::~PhongDistribution() {
    /* void */
}

manta::math::real manta::PhongDistribution::getPower(const IntersectionPoint *surfaceInteraction) {
    if (m_powerNode == nullptr) return m_power;

    const PhongMemory *memory = m_cache.cacheGet(surfaceInteraction->m_id, surfaceInteraction->m_threadId);

    if (memory == nullptr) {
        // There was a cache miss
        PhongMemory *newMemory = m_cache.cachePut(surfaceInteraction->m_id, surfaceInteraction->m_threadId);
        math::Vector sampledPower = math::constants::One;
        if (m_powerNode != nullptr) {
            // Sample the power input and save it in the state container
            VectorNodeOutput *powerNode = static_cast<VectorNodeOutput *>(m_powerNode);
            powerNode->sample(surfaceInteraction, (void *)&sampledPower);
        }

        math::real power = math::getScalar(sampledPower);
        newMemory->power = power * (m_power - m_minMapPower) + m_minMapPower;

        memory = newMemory;
    }

    return memory->power;
}

manta::math::Vector manta::PhongDistribution::generateMicrosurfaceNormal(
    const IntersectionPoint *surfaceInteraction) 
{
    math::real power = getPower(surfaceInteraction);

    math::real r1 = math::uniformRandom();
    math::real r2 = math::uniformRandom();

    math::real rho_m = math::constants::TWO_PI * r2;
    math::real cos_theta_m = ::pow(r1, (math::real)1.0 / (power + (math::real)2.0));
    math::real sin_theta_m = ::sqrt((math::real)1.0 - cos_theta_m * cos_theta_m);

    assert(!std::isnan(cos_theta_m));
    assert(!std::isnan(sin_theta_m));

    math::Vector t1 = math::loadVector(sin_theta_m, sin_theta_m, cos_theta_m);
    math::Vector t2 = math::loadVector(cos(rho_m), sin(rho_m), (math::real)1.0);

    return math::mul(t1, t2);
}

manta::math::real manta::PhongDistribution::calculateDistribution(
        const math::Vector &m, const IntersectionPoint *surfaceInteraction) 
{
    math::real power = getPower(surfaceInteraction);

    math::real cos_theta_m = math::getZ(m);
    if (cos_theta_m <= 0) return (math::real)0.0;

    math::real d_m = ((power + (math::real)2.0) / math::constants::TWO_PI) * ::pow(cos_theta_m, power);
    return d_m;
}

manta::math::real manta::PhongDistribution::calculateG1(
    const math::Vector &v, const math::Vector &m, const IntersectionPoint *surfaceInteraction) 
{
    math::real power = getPower(surfaceInteraction);

    math::real v_dot_m = (math::getScalar(math::dot(v, m)));
    math::real v_dot_n = (math::getZ(v));
    if ((v_dot_m < 0) != (v_dot_n < 0)) return (math::real)0.0;

    math::real cos_theta_v = ::abs(math::getZ(v));
    math::real sin2_theta_v = 1 - cos_theta_v * cos_theta_v;

    if (sin2_theta_v == 0) return 1.0;

    // tan_theta_v term is split to take advantage of the sqrt already here
    // Full expression: a = sqrt(0.5 * power + 1) / tan_theta_v
    math::real a = ::sqrt(((math::real)0.5 * power + 1) / sin2_theta_v) * cos_theta_v;

    math::real secondTerm = (math::real)1.0;
    if (a < (math::real)1.6) {
        secondTerm = (math::real)((3.535 * a + 2.181 * a * a) / (1 + 2.767 * a + 2.577 * a * a));
    }

    assert(a >= 0);
    assert(secondTerm >= 0);

    return secondTerm;
}

void manta::PhongDistribution::registerInputs() {
    registerInput(&m_powerNode, "power");
}
