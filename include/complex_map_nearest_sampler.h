#ifndef MANTARAY_COMPLEX_MAP_NEAREST_SAMPLER_H
#define MANTARAY_COMPLEX_MAP_NEAREST_SAMPLER_H

#include "complex_map_sampler.h"

#include "complex_map_2d.h"

#include <cmath>

namespace manta {

    class ComplexMapNearestSampler : public ComplexMapSampler {
    public:
        ComplexMapNearestSampler() {
            /* void */
        }

        virtual ~ComplexMapNearestSampler() {
            /* void */
        }

        virtual math::Complex sample(ComplexMap2D *map, math::real u, math::real v) {
            const int nearest_u = (int)std::round(u * map->getWidth());
            const int nearest_v = (int)std::round(v * map->getHeight());

            return map->get(nearest_u, nearest_v);
        }
    };

} /* namespace manta */

#endif /* MANTARAY_COMPLEX_MAP_NEAREST_SAMPLER_H */
