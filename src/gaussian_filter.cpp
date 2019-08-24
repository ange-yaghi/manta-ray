#include "../include/gaussian_filter.h"

#include <algorithm>

manta::GaussianFilter::GaussianFilter() {
    m_alpha = (math::real)0.0;
    m_expX = (math::real)0.0;
    m_expY = (math::real)0.0;
}

manta::GaussianFilter::~GaussianFilter() {
    /* void */
}

void manta::GaussianFilter::configure(math::real alpha) {
    m_alpha = alpha;
    m_expX = std::exp(-m_alpha * m_extents.x * m_extents.x);
    m_expY = std::exp(-m_alpha * m_extents.y * m_extents.y);
}

manta::math::Vector manta::GaussianFilter::evaluate(const math::Vector2 &p) {
    return math::loadScalar(
        gaussian(p.x, m_expX) * gaussian(p.y, m_expY));
}

manta::math::real manta::GaussianFilter::gaussian(math::real d, math::real expv) {
    return std::max((math::real)0.0, (math::real)(std::exp(-m_alpha * d * d) - expv));
}
