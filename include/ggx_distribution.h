#ifndef MANTARAY_GGX_DISTRIBUTION_H
#define MANTARAY_GGX_DISTRIBUTION_H

#include "microfacet_distribution.h"

#include "vector_node_output.h"
#include "node_cache.h"

namespace manta {

    // Forward declarations
    class VectorNodeOutput;

    class GgxDistribution : public MicrofacetDistribution {
    public:
        static constexpr bool ENABLE_OPTIMIZATION = true;

    public:
        struct GgxMemory {
            math::real width;
        };

    public:
        GgxDistribution();
        virtual ~GgxDistribution();

        math::real getWidth(const IntersectionPoint *surfaceInteraction);

        virtual math::Vector generateMicrosurfaceNormal(const IntersectionPoint *surfaceInteraction);
        virtual math::real calculateDistribution(const math::Vector &m, 
            const IntersectionPoint *surfaceInteraction);
        virtual math::real calculateG1(const math::Vector &v, const math::Vector &m, 
            const IntersectionPoint *surfaceInteraction);

        void setWidth(math::real width) { m_width = width; }
        math::real getWidth() const { return m_width; }

        void setWidthNode(piranha::pNodeInput node) { m_widthNode = node; }
        piranha::pNodeInput getWidthNode() const { return m_widthNode; }

        void setMinMapWidth(math::real power) { m_minMapWidth = power; }
        math::real getMinMapWidth() const { return m_minMapWidth; }

    public:
        static math::Vector generateMicrosurfaceNormal(math::real width);
        static math::real calculateDistribution(const math::Vector &m, math::real width);
        static math::real calculateG1(const math::Vector &v, const math::Vector &m, math::real width);

    protected:
        virtual void registerInputs();

        virtual void _evaluate();
        virtual piranha::Node *_optimize();

    protected:
        piranha::pNodeInput m_widthNode;
        bool m_useNodes;

        math::real m_width;
        math::real m_minMapWidth;

        NodeCache<GgxMemory> m_cache;
    };

} /* namespace manta */

#endif /* MANTARAY_GGX_DISTRIBUTION_H */
