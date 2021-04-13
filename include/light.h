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

        virtual math::Vector sampleIncoming(const IntersectionPoint &ref, const math::Vector2 &u, math::Vector *wi, math::real *pdf, math::real *depth) const = 0;
        virtual math::real pdfIncoming(const IntersectionPoint &ref, const math::Vector &wi) const = 0;

        virtual math::Vector L(const IntersectionPoint &ref, const math::Vector &wi) const = 0;

        virtual bool intersect(const math::Vector &src, const math::Vector &dir, math::real *depth) const = 0;
    };

} /* namespace manta */

#endif /* MANTARAY_LIGHT_H */
