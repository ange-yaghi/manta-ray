#ifndef MANTARAY_LIGHT_H
#define MANTARAY_LIGHT_H

#include "manta_math.h"
#include "intersection_point.h"

namespace manta {

    class Scene;

    class Light {
    public:
        Light();
        ~Light();

        virtual math::Vector sampleIncoming(const IntersectionPoint &ref, const math::Vector2 &u, math::Vector *wi, math::real *pdf) const = 0;
        virtual math::real pdfIncoming(const IntersectionPoint &ref, const math::Vector &wi) const = 0;
    };

} /* namespace manta */

#endif /* MANTARAY_LIGHT_H */
