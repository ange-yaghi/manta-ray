#ifndef MANTARAY_GGX_DISTRIBUTION_H
#define MANTARAY_GGX_DISTRIBUTION_H

#include "microfacet_distribution.h"

#include "vector_node_output.h"
#include "cacheable_input.h"
#include "node_cache.h"

namespace manta {

    // Forward declarations
    class VectorNodeOutput;

    class GgxDistribution : public MicrofacetDistribution {
    public:
        static constexpr bool ENABLE_OPTIMIZATION = true;

    public:
        GgxDistribution();
        virtual ~GgxDistribution();

        math::real getWidth(const IntersectionPoint *surfaceInteraction);

        virtual math::Vector generateMicrosurfaceNormal(
            const IntersectionPoint *surfaceInteraction, const math::Vector2 &u);
        virtual math::real calculateDistribution(const math::Vector &m, 
            const IntersectionPoint *surfaceInteraction);
        virtual math::real calculateG1(const math::Vector &v, const math::Vector &m, 
            const IntersectionPoint *surfaceInteraction);

        void setWidth(math::real width) { m_width.setConstant(math::loadVector(width)); }
        math::real getWidth() const { return math::getScalar(m_width.getConstant()); }

        void setWidthNode(piranha::pNodeInput node) { m_width.setPort(node); }
        piranha::pNodeInput getWidthNode() const { return m_width.getPort(); }

    public:
        static math::Vector generateMicrosurfaceNormal(math::real width, const math::Vector2 &u);
        static math::real calculateDistribution(const math::Vector &m, math::real width);
        static math::real calculateG1(const math::Vector &v, const math::Vector &m, math::real width);

    protected:
        virtual void registerInputs();

        virtual void _evaluate();
        virtual piranha::Node *_optimize(piranha::NodeAllocator *nodeAllocator);

    protected:
        CacheableInput<math::Vector> m_width;
        NodeCache<math::real> m_distribution;
    };

} /* namespace manta */

#endif /* MANTARAY_GGX_DISTRIBUTION_H */
