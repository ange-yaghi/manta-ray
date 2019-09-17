#include "../include/intersection_point.h"

void manta::IntersectionPoint::calculateCachedValues() {
    // Generate basis vectors
    math::Vector m_vertexU = (abs(math::getX(m_vertexNormal)) < 0.1f)
        ? math::constants::XAxis
        : math::constants::YAxis;
    math::Vector m_vertexV;
    math::Vector w = m_vertexNormal;
    
    m_vertexU = math::normalize(math::cross(m_vertexU, m_vertexNormal));
    m_vertexV = math::cross(m_vertexNormal, m_vertexU);
}

manta::math::Vector manta::IntersectionPoint::worldToLocal(const math::Vector &i) const {
    math::Vector t_dir = math::loadVector(
        math::getScalar(math::dot(i, m_vertexU)),
        math::getScalar(math::dot(i, m_vertexV)),
        math::getScalar(math::dot(i, m_vertexNormal)));

    return t_dir;
}

bool manta::IntersectionPoint::isReflection(const math::Vector &i, const math::Vector &o) const {
    return math::getScalar(math::mul(
        math::dot(i, m_faceNormal),
        math::dot(i, m_vertexNormal)
    )) > 0;
}
