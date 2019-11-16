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
        MicrofacetDistribution();
        virtual ~MicrofacetDistribution();

        virtual math::Vector generateMicrosurfaceNormal(
            const IntersectionPoint *surfaceInteraction, const math::Vector2 &u) = 0;
        virtual math::real calculatePDF(const math::Vector &m, const IntersectionPoint *surfaceInteraction);
        virtual math::real calculateDistribution(const math::Vector &m, 
            const IntersectionPoint *surfaceInteraction) = 0;
        virtual math::real calculateG1(const math::Vector &v, const math::Vector &m, 
            const IntersectionPoint *surfaceInteraction) = 0;
        virtual math::real bidirectionalShadowMasking(const math::Vector &i, 
            const math::Vector &o, const math::Vector &m, const IntersectionPoint *surfaceInteraction);

        math::real smithBidirectionalShadowMasking(const math::Vector &i, const math::Vector &o, 
            const math::Vector &m, const IntersectionPoint *surfaceInteraction);

        ObjectReferenceNodeOutput<MicrofacetDistribution> *getMainOutput() { return &m_output; }
    };

} /* namespace manta */

#endif /* MANTARAY_MICROFACET_DISTRIBUTION_H */
