#include "../include/gaussian_filter.h"

#include "../include/standard_allocator.h"

#include <algorithm>

manta::GaussianFilter::GaussianFilter() {
    m_alpha = (math::real)0.0;
    m_expX = (math::real)0.0;
    m_expY = (math::real)0.0;

    m_cache = nullptr;
    m_alphaInput = nullptr;
}

manta::GaussianFilter::~GaussianFilter() {
    /* void */
}

void manta::GaussianFilter::configure(math::real alpha) {
    m_alpha = alpha;
    m_expX = std::exp(-m_alpha * m_extents.x * m_extents.x);
    m_expY = std::exp(-m_alpha * m_extents.y * m_extents.y);
    m_invR = 1 / m_extents.x;

    if (ENABLE_CACHE) {
        computeCache();
    }
}

manta::math::Vector manta::GaussianFilter::evaluate(const math::Vector2 &p) {
    return (m_cache == nullptr)
        ? math::loadScalar(gaussian(p.x, m_expX) * gaussian(p.y, m_expY))
        : math::loadScalar(gaussianCache(p.x) * gaussianCache(p.y));
}

manta::math::real manta::GaussianFilter::gaussian(math::real d, math::real expv) {
    return std::max((math::real)0.0, (math::real)(std::exp(-m_alpha * d * d) - expv));
}

manta::math::real manta::GaussianFilter::gaussianCache(math::real d) {
    constexpr int actualSteps = CACHE_STEPS - SAFETY_FACTOR;
    const int index = (int)(actualSteps * (std::abs(d) * m_invR));
    return m_cache[index];
}

void manta::GaussianFilter::computeCache() {
    constexpr int actualSteps = CACHE_STEPS - SAFETY_FACTOR;
    constexpr math::real step = (math::real)1.0 / actualSteps;

    m_cache = StandardAllocator::Global()->allocate<math::real>(CACHE_STEPS);
    for (int i = 0; i <= actualSteps; i++) {
        math::real s = step * i * m_extents.x;
        m_cache[i] = gaussian(s, m_expX);
    }

    for (int i = actualSteps + 1; i < CACHE_STEPS; i++) {
        m_cache[i] = (math::real)0.0;
    }
}

void manta::GaussianFilter::_initialize() {
    Filter::_initialize();
}

void manta::GaussianFilter::_evaluate() {
    Filter::_evaluate();

    piranha::native_float alpha;
    m_alphaInput->fullCompute((void *)&alpha);

    configure((math::real)alpha);
}

void manta::GaussianFilter::registerInputs() {
    Filter::registerInputs();

    registerInput(&m_alphaInput, "alpha");
}
