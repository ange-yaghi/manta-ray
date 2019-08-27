#ifndef MANTARAY_GAUSSIAN_FILTER_H
#define MANTARAY_GAUSSIAN_FILTER_H

#include "filter.h"

namespace manta {

    class GaussianFilter : public Filter {
    protected:
        static constexpr int CACHE_STEPS = 1024;
        static constexpr int SAFETY_FACTOR = 32;
        static constexpr bool ENABLE_CACHE = true;

    public:
        GaussianFilter();
        ~GaussianFilter();

        void configure(math::real alpha);

        virtual math::Vector evaluate(const math::Vector2 &p);

    protected:
        math::real gaussian(math::real d, math::real expv);
        math::real gaussianCache(math::real d);
        void computeCache();

        math::real m_alpha;

    protected:
        math::real m_expX;
        math::real m_expY;
        math::real m_invR;

    protected:
        math::real *m_cache;

    protected:
        virtual void _initialize();
        virtual void _evaluate();
        virtual void registerInputs();

        piranha::pNodeInput m_alphaInput;
    };

} /* namespace manta */

#endif /* MANTARAY_GAUSSIAN_FILTER_H */
