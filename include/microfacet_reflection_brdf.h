#ifndef MANTARAY_MICROFACET_REFLECTION_BRDF_H
#define MANTARAY_MICROFACET_REFLECTION_BRDF_H

#include "bxdf.h"

namespace manta {

    class MicrofacetDistribution;
    class MediaInterface;

    class MicrofacetReflectionBSDF : public BXDF {
    public:
        MicrofacetReflectionBSDF();
        virtual ~MicrofacetReflectionBSDF();

        virtual math::Vector sampleF(const IntersectionPoint *surfaceInteraction, 
            const math::Vector &i, math::Vector *o, math::real *pdf, 
            StackAllocator *stackAllocator) const;
        virtual math::Vector f(const IntersectionPoint *surfaceInteraction,
            const math::Vector &i, const math::Vector &o, StackAllocator *stackAllocator) const;
        virtual math::real calculatePDF(const IntersectionPoint *surfaceInteraction, 
            const math::Vector &i, const math::Vector &o, StackAllocator *stackAllocator) const;

        void setDistribution(MicrofacetDistribution *distribution) { m_distribution = distribution; }
        const MicrofacetDistribution *getDistribution() const { return m_distribution; }

    protected:
        virtual void _evaluate();

    protected:
        piranha::pNodeInput m_distributionInput;
        MicrofacetDistribution *m_distribution;
        
        virtual void registerInputs();
    };

} /* namespace manta */

#endif /* MANTARAY_MICROFACET_REFLECTION_BRDF_H */
