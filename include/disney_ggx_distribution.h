#ifndef MANTARAY_DISNEY_GGX_DISTRIBUTION_H
#define MANTARAY_DISNEY_GGX_DISTRIBUTION_H

#include "microfacet_distribution.h"

#include "node_cache.h"

namespace manta {

    // Forward declarations
    class VectorNodeOutput;

    class DisneyGgxDistribution : public MicrofacetDistribution {
    public:
        static constexpr bool ENABLE_OPTIMIZATION = true;

    public:
        struct DisneyGgxMemory {
            math::real roughness = (math::real)0.0;

            bool calculatedDistribution = false;
            math::real distribution = (math::real)0.0;
        };

    public:
        DisneyGgxDistribution();
        virtual ~DisneyGgxDistribution();

        math::real getRoughness(const IntersectionPoint *surfaceInteraction);
        math::real getAlpha(const IntersectionPoint *surfaceInteraction);

        virtual math::Vector generateMicrosurfaceNormal(const IntersectionPoint *surfaceInteraction);
        virtual math::real calculateDistribution(const math::Vector &m,
            const IntersectionPoint *surfaceInteraction);
        virtual math::real calculateG1(const math::Vector &v, const math::Vector &m,
            const IntersectionPoint *surfaceInteraction);

        void setRoughness(math::real roughness) { m_roughness = roughness; }
        math::real getRoughness() const { return m_roughness; }

        void setRoughnessNode(piranha::pNodeInput node) { m_roughnessNode = node; }
        piranha::pNodeInput getWidthNode() const { return m_roughnessNode; }

    protected:
        piranha::pNodeInput m_roughnessNode;

        virtual void registerInputs();

        virtual void _evaluate();
        virtual piranha::Node *_optimize();

        bool m_useNodes;

    protected:
        math::real m_roughness;

        NodeCache<DisneyGgxMemory> m_cache;
    };

} /* namespace manta */

#endif /* MANTARAY_DISNEY_GGX_DISTRIBUTION_H */
