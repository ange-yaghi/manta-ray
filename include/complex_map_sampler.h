#ifndef MANTARAY_COMPLEX_MAP_SAMPLER_H
#define MANTARAY_COMPLEX_MAP_SAMPLER_H

#include "complex_math.h"

namespace manta {

    class ComplexMap2D;

    class ComplexMapSampler {
    public:
        ComplexMapSampler() {
            /* void */
        }

        virtual ~ComplexMapSampler() {
            /* void */
        }

        virtual math::Complex sample(ComplexMap2D *map, math::real u, math::real v) = 0;
    };

} /* namespace manta */

#endif /* MANTARAY_COMPLEX_MAP_SAMPLER_H */
