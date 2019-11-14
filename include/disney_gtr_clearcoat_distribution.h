#ifndef MANTARAY_DISNEY_GTR_CLEARCOAT_DISTRIBUTION_H
#define MANTARAY_DISNEY_GTR_CLEARCOAT_DISTRIBUTION_H

#include "microfacet_distribution.h"

#include "cacheable_input.h"
#include "node_cache.h"

namespace manta {

    // Forward declarations
    class VectorNodeOutput;

    class DisneyGtrClearcoatDistribution : public MicrofacetDistribution {
    public:
        static constexpr bool ENABLE_OPTIMIZATION = true;

    public:
        DisneyGtrClearcoatDistribution();
        virtual ~DisneyGtrClearcoatDistribution();

        math::real getRoughness(const IntersectionPoint *surfaceInteraction);
        math::real getAlpha(const IntersectionPoint *surfaceInteraction);

        virtual math::Vector generateMicrosurfaceNormal(const IntersectionPoint *surfaceInteraction);
        virtual math::real calculateDistribution(const math::Vector &m,
            const IntersectionPoint *surfaceInteraction);
        virtual math::real calculateG1(const math::Vector &v, const math::Vector &m,
            const IntersectionPoint *surfaceInteraction);

        void setRoughness(math::real roughness) { m_roughness.setDefault(math::loadScalar(roughness)); }
        math::real getRoughness() const { return math::getScalar(m_roughness.getConstant()); }

        void setRoughnessNode(piranha::pNodeInput node) { m_roughness.setPort(node); }
        piranha::pNodeInput getWidthNode() const { return m_roughness.getPort(); }

    protected:
        virtual void registerInputs();

        virtual void _evaluate();
        virtual piranha::Node *_optimize();

    protected:
        CacheableInput<math::Vector> m_roughness;
    };

} /* namespace manta */

#endif /* MANTARAY_DISNEY_GTR_CLEARCOAT_DISTRIBUTION_H */
