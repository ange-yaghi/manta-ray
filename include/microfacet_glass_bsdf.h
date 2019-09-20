#ifndef MANTARAY_MICROFACET_GLASS_BSDF_H
#define MANTARAY_MICROFACET_GLASS_BSDF_H

#include "bxdf.h"

namespace manta {

    class MicrofacetDistribution;
    class MediaInterface;

    class MicrofacetGlassBSDF : public BXDF {
    public:
        MicrofacetGlassBSDF();
        virtual ~MicrofacetGlassBSDF();

        virtual math::Vector sampleF(const IntersectionPoint *surfaceInteraction, 
            const math::Vector &i, math::Vector *o, math::real *pdf, 
            StackAllocator *stackAllocator) const;
        virtual math::Vector f(const IntersectionPoint *surfaceInteraction,
            const math::Vector &i, const math::Vector &o, StackAllocator *stackAllocator) const;
        virtual math::real pdf(const IntersectionPoint *surfaceInteraction,
            const math::Vector &i, const math::Vector &o) const;

        void setDistribution(MicrofacetDistribution *distribution)  { m_distribution = distribution; }
        MicrofacetDistribution *getDistribution() const { return m_distribution; }

        void setMediaInterface(MediaInterface *mediaInterface) { m_mediaInterface = mediaInterface; }
        MediaInterface *getMediaInterface() { return m_mediaInterface; }

    protected:
        MicrofacetDistribution *m_distribution;
        MediaInterface *m_mediaInterface;

    protected:
        virtual void _evaluate();

        virtual void registerInputs();

        piranha::pNodeInput m_distributionInput;
        piranha::pNodeInput m_mediaInterfaceInput;
    };

} /* namespace manta */

#endif /* MANTARAY_MICROFACET_GLASS_BSDF_H */

