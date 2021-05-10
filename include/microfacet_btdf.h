#ifndef MANTARAY_MICROFACET_BTDF_H
#define MANTARAY_MICROFACET_BTDF_H

#include "bxdf.h"

namespace manta {

    class MicrofacetDistribution;
    class MediaInterface;

    class MicrofacetBTDF : public BXDF {
    public:
        MicrofacetBTDF();
        virtual ~MicrofacetBTDF();

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
        MicrofacetDistribution *getDistribution() const { return m_distribution; }

        void setMediaInterface(MediaInterface *mediaInterface) { m_mediaInterface = mediaInterface; }
        MediaInterface *getMediaInterface() { return m_mediaInterface; }

    protected:
        virtual void _evaluate();
        virtual piranha::Node *_optimize(piranha::NodeAllocator *nodeAllocator);
        virtual void registerInputs();

        piranha::pNodeInput m_distributionInput;
        piranha::pNodeInput m_mediaInterfaceInput;

        MicrofacetDistribution *m_distribution;
        MediaInterface *m_mediaInterface;

        CacheableInput<math::Vector> m_transmittance;
    };

} /* namespace manta */

#endif /* MANTARAY_MICROFACET_BTDF_H */
