#ifndef MANTARAY_MICROFACET_BRDF_H
#define MANTARAY_MICROFACET_BRDF_H

#include "bxdf.h"

#include "cacheable_input.h"

namespace manta {

    class MicrofacetDistribution;
    class MediaInterface;

    class MicrofacetBRDF : public BXDF {
    public:
        static constexpr math::real MinEpsilon = (math::real)0.0;

    public:
        MicrofacetBRDF();
        virtual ~MicrofacetBRDF();

        virtual math::Vector sampleF(
            const IntersectionPoint *surfaceInteraction, 
            const math::Vector2 &u,
            const math::Vector &i,
            math::Vector *o,
            math::real *pdf,
            RayFlags *flags,
            StackAllocator *stackAllocator);

        virtual math::Vector f(
            const IntersectionPoint *surfaceInteraction,
            const math::Vector &i,
            const math::Vector &o,
            StackAllocator *stackAllocator);

        virtual math::real pdf(
            const IntersectionPoint *surfaceInteraction,
            const math::Vector &i,
            const math::Vector &o);

        void setDistribution(MicrofacetDistribution *distribution) { m_distribution = distribution; }
        const MicrofacetDistribution *getDistribution() const { return m_distribution; }

        void setBaseReflectivity(const math::Vector &reflectivity);
        math::Vector getReflectivity(const IntersectionPoint *surfaceInteraction);

    protected:
        virtual void _evaluate();
        virtual piranha::Node *_optimize(piranha::NodeAllocator *nodeAllocator);
        virtual void registerInputs();

        piranha::pNodeInput m_distributionInput;
        piranha::pNodeInput m_mediaInterfaceInput;
        CacheableInput<math::Vector> m_reflectivity;

        MediaInterface *m_mediaInterface;
        MicrofacetDistribution *m_distribution;
    };

} /* namespace manta */

#endif /* MANTARAY_MICROFACET_BRDF_H */
