#include "../include/triangle_filter.h"

#include <algorithm>

manta::TriangleFilter::TriangleFilter() {
    /* void */
}

manta::TriangleFilter::~TriangleFilter() {
    /* void */
}

manta::math::Vector manta::TriangleFilter::evaluate(const math::Vector2 &p) {
    math::real v = std::max((math::real)0.0, m_extents.x - std::abs(p.x)) *
        std::max((math::real)0.0, m_extents.y - std::abs(p.y));

    return math::loadScalar(v);
}
