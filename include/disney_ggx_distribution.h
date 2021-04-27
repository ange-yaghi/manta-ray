#ifndef MANTARAY_DISNEY_GGX_DISTRIBUTION_H
#define MANTARAY_DISNEY_GGX_DISTRIBUTION_H

#include "microfacet_distribution.h"

#include "node_cache.h"
#include "cacheable_input.h"

namespace manta {

    // Forward declarations
    class VectorNodeOutput;

    class DisneyGgxDistribution : public MicrofacetDistribution {
    public:
        static constexpr bool EnableOptimization = true;
        static constexpr math::real DeltaThreshold = (math::real)1E-4;

    public:
        DisneyGgxDistribution();
        virtual ~DisneyGgxDistribution();

        math::real getRoughness(const IntersectionPoint *surfaceInteraction);
        math::real getAlpha(const IntersectionPoint *surfaceInteraction);

        virtual math::Vector generateMicrosurfaceNormal(
            const IntersectionPoint *surfaceInteraction,
            const math::Vector2 &u);
        virtual math::real calculateDistribution(
            const math::Vector &m,
            const IntersectionPoint *surfaceInteraction);
        virtual math::real calculateG1(
            const math::Vector &v,
            const math::Vector &m,
            const IntersectionPoint *surfaceInteraction);
        virtual bool isDelta(const IntersectionPoint *surfaceInteraction);

        void setRoughness(math::real roughness) { m_roughness.setConstant(math::loadScalar(roughness)); }
        math::real getRoughness() const { return math::getScalar(m_roughness.getConstant()); }

        void setRoughnessNode(piranha::pNodeInput node) { m_roughness.setPort(node); }
        piranha::pNodeInput getWidthNode() const { return m_roughness.getPort(); }

    public:
        __forceinline bool isDeltaInternal(const IntersectionPoint *surfaceInteraction) {
            return getRoughness(surfaceInteraction) < DeltaThreshold;
        }

    protected:
        virtual void registerInputs();

        virtual void _evaluate();
        virtual piranha::Node *_optimize(piranha::NodeAllocator *nodeAllocator);

    protected:
        CacheableInput<math::Vector> m_roughness;
        NodeCache<math::real, DistributionCacheKey> m_distribution;
    };

} /* namespace manta */

#endif /* MANTARAY_DISNEY_GGX_DISTRIBUTION_H */
