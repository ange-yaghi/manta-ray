#ifndef MANTARAY_PHONG_DISTRIBUTION_H
#define MANTARAY_PHONG_DISTRIBUTION_H

#include "microfacet_distribution.h"

#include "node_cache.h"

namespace manta {

    // Forward declarations
    class VectorNodeOutput;

    class PhongDistribution : public MicrofacetDistribution {
    public:
        struct PhongMemory {
            math::real power;
        };

    public:
        PhongDistribution();
        virtual ~PhongDistribution();

        math::real getPower(const IntersectionPoint *surfaceInteraction);

        virtual math::Vector generateMicrosurfaceNormal(const IntersectionPoint *surfaceInteraction);
        virtual math::real calculateDistribution(const math::Vector &m, 
            const IntersectionPoint *surfaceInteraction);
        virtual math::real calculateG1(const math::Vector &v, const math::Vector &m, 
            const IntersectionPoint *surfaceInteraction);

        void setPower(math::real power) { m_power = power; }
        math::real getPower() const { return m_power; }

        void setPowerNode(piranha::pNodeInput node) { m_powerNode = node; }
        piranha::pNodeInput getPowerNode() const { return m_powerNode; }

        void setMinMapPower(math::real power) { m_minMapPower = power; }
        math::real getMinMapPower() const { return m_minMapPower; }

    protected:
        virtual void registerInputs();

    protected:
        piranha::pNodeInput m_powerNode;
        math::real m_power;
        math::real m_minMapPower;

        NodeCache<PhongMemory> m_cache;
    };

} /* namespace manta */

#endif /* MANTARAY_PHONG_DISTRIBUTION_H */
