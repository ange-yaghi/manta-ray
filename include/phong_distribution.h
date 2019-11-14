#ifndef MANTARAY_PHONG_DISTRIBUTION_H
#define MANTARAY_PHONG_DISTRIBUTION_H

#include "microfacet_distribution.h"

#include "cacheable_input.h"

namespace manta {

    // Forward declarations
    class VectorNodeOutput;

    class PhongDistribution : public MicrofacetDistribution {
    public:
        PhongDistribution();
        virtual ~PhongDistribution();

        math::real getPower(const IntersectionPoint *surfaceInteraction);

        virtual math::Vector generateMicrosurfaceNormal(const IntersectionPoint *surfaceInteraction);
        virtual math::real calculateDistribution(const math::Vector &m, 
            const IntersectionPoint *surfaceInteraction);
        virtual math::real calculateG1(const math::Vector &v, const math::Vector &m, 
            const IntersectionPoint *surfaceInteraction);

        void setPower(math::real power) { m_power.setDefault(math::loadScalar(power)); }
        math::real getPower() const { return math::getScalar(m_power.getDefault()); }

        void setPowerNode(piranha::pNodeInput node) { m_power.setPort(node); }
        piranha::pNodeInput getPowerNode() const { return m_power.getPort(); }

    protected:
        virtual void _evaluate();
        virtual void registerInputs();

    protected:
        CacheableInput<math::Vector> m_power;
    };

} /* namespace manta */

#endif /* MANTARAY_PHONG_DISTRIBUTION_H */
