#ifndef MANTARAY_MICROFACET_REFLECTION_BRDF_H
#define MANTARAY_MICROFACET_REFLECTION_BRDF_H

#include "bxdf.h"

namespace manta {

    class MicrofacetDistribution;
    class MediaInterface;

    class MicrofacetReflectionBSDF : public BXDF {
    public:
        static constexpr math::real MIN_EPSILON = (math::real)0.0;

    public:
        MicrofacetReflectionBSDF();
        virtual ~MicrofacetReflectionBSDF();

        virtual math::Vector sampleF(const IntersectionPoint *surfaceInteraction, 
            const math::Vector &i, math::Vector *o, math::real *pdf, 
            StackAllocator *stackAllocator) const;
        virtual math::Vector f(const IntersectionPoint *surfaceInteraction,
            const math::Vector &i, const math::Vector &o, StackAllocator *stackAllocator) const;
        virtual math::real pdf(const IntersectionPoint *surfaceInteraction,
            const math::Vector &i, const math::Vector &o) const;

        void setDistribution(MicrofacetDistribution *distribution) { m_distribution = distribution; }
        const MicrofacetDistribution *getDistribution() const { return m_distribution; }

        math::Vector getReflectivity(const IntersectionPoint *surfaceInteraction) const;

    protected:
        virtual void _evaluate();
        virtual void registerInputs();

        piranha::pNodeInput m_distributionInput;
        piranha::pNodeInput m_reflectivityInput;

        MicrofacetDistribution *m_distribution;
        math::Vector m_baseReflectivity;
    };

} /* namespace manta */

#endif /* MANTARAY_MICROFACET_REFLECTION_BRDF_H */
