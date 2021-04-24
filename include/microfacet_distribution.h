#ifndef MANTARAY_MICROFACET_DISTRIBUTION_H
#define MANTARAY_MICROFACET_DISTRIBUTION_H

#include "node.h"

#include "manta_math.h"
#include "intersection_point.h"
#include "object_reference_node.h"

namespace manta {

    // Forward declarations
    class StackAllocator;

    class MicrofacetDistribution : public ObjectReferenceNode<MicrofacetDistribution> {
    public:
        static constexpr math::Vector SurfaceAlignedMicrofacetNormal = math::constants::ZAxis;

    public:
        MicrofacetDistribution();
        virtual ~MicrofacetDistribution();

        virtual math::Vector generateMicrosurfaceNormal(
            const IntersectionPoint *surfaceInteraction,
            const math::Vector2 &u) = 0;
        virtual math::real calculatePDF(
            const math::Vector &m,
            const IntersectionPoint *surfaceInteraction);
        virtual math::real calculateDistribution(
            const math::Vector &m,
            const IntersectionPoint *surfaceInteraction) = 0;
        virtual math::real calculateG1(
            const math::Vector &v,
            const math::Vector &m,
            const IntersectionPoint *surfaceInteraction) = 0;
        virtual math::real bidirectionalShadowMasking(
            const math::Vector &i,
            const math::Vector &o,
            const math::Vector &m,
            const IntersectionPoint *surfaceInteraction);

        virtual bool isDelta(const IntersectionPoint *surfaceInteraction) { return false; }

        math::real smithBidirectionalShadowMasking(
            const math::Vector &i,
            const math::Vector &o,
            const math::Vector &m,
            const IntersectionPoint *surfaceInteraction);

        ObjectReferenceNodeOutput<MicrofacetDistribution> *getMainOutput() { return &m_output; }

    protected:
        struct DistributionCacheKey {
            math::Vector m = math::constants::Zero;
            intersection_id id = -1;

            bool operator==(const DistributionCacheKey &key) const {
                return
                    key.id == id &&
                    math::bitwiseEqual(key.m, m);
            }
        };
    };

} /* namespace manta */

#endif /* MANTARAY_MICROFACET_DISTRIBUTION_H */
