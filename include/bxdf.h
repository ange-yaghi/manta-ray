#ifndef MANTARAY_BXDF_H
#define MANTARAY_BXDF_H

#include "node.h"

#include "manta_math.h"
#include "media_interface.h"
#include "object_reference_node.h"

#include <algorithm>
#include <math.h>

namespace manta {

    struct IntersectionPoint;
    class StackAllocator;

    class BXDF : public ObjectReferenceNode<BXDF> {
    public:
        BXDF();
        virtual ~BXDF();

        virtual math::Vector sampleF(const IntersectionPoint *surfaceInteraction,
            const math::Vector &i, math::Vector *o, math::real *pdf,
            StackAllocator *stackAllocator) = 0;

        virtual math::Vector f(const IntersectionPoint *surfaceInteraction,
            const math::Vector &i, const math::Vector &o, StackAllocator *stackAllocator) = 0;

        virtual math::real pdf(const IntersectionPoint *surfaceInteraction,
            const math::Vector &i, const math::Vector &o) = 0;

        static inline bool refract(const math::Vector &i, const math::Vector &n,
            math::real ior, math::Vector *t);

    protected:
        virtual void _evaluate();
    };

    inline bool BXDF::refract(const math::Vector &i, const math::Vector &n,
        math::real ior, math::Vector *t) {
        math::real cosThetaI = math::getScalar(math::dot(n, i));
        math::real sin2ThetaI = std::max((math::real)0.0, (math::real)1.0 - cosThetaI * cosThetaI);
        math::real sin2ThetaT = ior * ior * sin2ThetaI;

        if (sin2ThetaT >= (math::real)1.0) return false;

        math::real cosThetaT = ::sqrt((math::real)1.0 - sin2ThetaT);

        math::Vector ior_v = math::loadScalar(ior);
        math::Vector S = math::loadScalar(ior * cosThetaI - cosThetaT);
        *t = math::add(
            math::mul(ior_v, math::negate(i)),
            math::mul(S, n)
        );

        return true;
    }

} /* namespace manta */

#endif /* MANTARAY_BXDF_H */
