#ifndef MANTARAY_MICROFACET_REFLECTION_BSDF_H
#define MANTARAY_MICROFACET_REFLECTION_BSDF_H

#include "bsdf.h"

namespace manta {

    class MicrofacetDistribution;
    class MediaInterface;

    class MicrofacetReflectionBSDF : public BSDF {
    public:
        MicrofacetReflectionBSDF();
        virtual ~MicrofacetReflectionBSDF();

        virtual void initializeSessionMemory(const IntersectionPoint *surfaceInteraction, 
            NodeSessionMemory *memory, StackAllocator *stackAllocator) const;

        virtual math::Vector sampleF(const IntersectionPoint *surfaceInteraction, 
            const math::Vector &i, math::Vector *o, math::real *pdf, 
            StackAllocator *stackAllocator) const;
        virtual math::real calculatePDF(const IntersectionPoint *surfaceInteraction, 
            const math::Vector &i, const math::Vector &o, StackAllocator *stackAllocator) const;

        void setDistribution(const MicrofacetDistribution *distribution) { m_distribution = distribution; }
        const MicrofacetDistribution *getDistribution() const { return m_distribution; }

    protected:
        virtual void _evaluate();

    protected:
        piranha::pNodeInput m_distributionInput;
        const MicrofacetDistribution *m_distribution;
        
        virtual void registerInputs();
    };

} /* namespace manta */

#endif /* MANTARAY_MICROFACET_REFLECTION_BSDF_H */
