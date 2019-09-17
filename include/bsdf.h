#ifndef MANTARAY_BSDF_H
#define MANTARAY_BSDF_H

#include "node.h"

#include "manta_math.h"
#include "media_interface.h"
#include "object_reference_node.h"

#include <algorithm>
#include <math.h>

namespace manta {

    struct IntersectionPoint;
    class StackAllocator;
    class BXDF;

    class BSDF : public ObjectReferenceNode<BSDF> {
    public:
        static constexpr int MAX_BXDFS = 8;

    public:
        BSDF();
        BSDF(BXDF *bxdf);
        virtual ~BSDF();

        math::Vector sampleF(const IntersectionPoint *surfaceInteraction, 
            const math::Vector &i, math::Vector *o, math::real *pdf, 
            StackAllocator *stackAllocator) const;

        math::Vector f(const IntersectionPoint *surfaceInteraction, const math::Vector &i, const math::Vector &o) const;

        static inline bool refract(const math::Vector &i, const math::Vector &n,
            math::real ior, math::Vector *t);

        int getBxdfCount() const { return m_bxdfCount; }
        void addBxdf(BXDF *bxdf) { m_bxdfs[m_bxdfCount++] = bxdf; }

    protected:
        BXDF *m_bxdfs[MAX_BXDFS];
        int m_bxdfCount;
    };

    inline bool BSDF::refract(const math::Vector &i, const math::Vector &n,
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

#endif /* MANTARAY_BSDF_H */
