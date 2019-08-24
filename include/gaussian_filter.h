#ifndef MANTARAY_GAUSSIAN_FILTER_H
#define MANTARAY_GAUSSIAN_FILTER_H

#include "filter.h"

namespace manta {

    class GaussianFilter : public Filter {
    public:
        GaussianFilter();
        ~GaussianFilter();

        void configure(math::real alpha);

        virtual math::Vector evaluate(const math::Vector2 &p);

    protected:
        math::real gaussian(math::real d, math::real expv);

        math::real m_alpha;
        math::real m_expX;
        math::real m_expY;
    };

} /* namespace manta */

#endif /* MANTARAY_GAUSSIAN_FILTER_H */
