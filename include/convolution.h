#ifndef MANTARAY_CONVOLUTION_H
#define MANTARAY_CONVOLUTION_H

#include "manta_math.h"

namespace manta {

    class Convolution {
    public:
        Convolution();
        ~Convolution();

        virtual math::real getExtents(const math::Vector &reference) const = 0;
        virtual math::Vector sample(math::real x, math::real y) const = 0;

        virtual void destroy() {}
    };

} /* namespace manta */

#endif /* MANTARAY_CONVOLUTION_H */
