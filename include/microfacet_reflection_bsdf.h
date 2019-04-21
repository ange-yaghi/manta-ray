#ifndef MICROFACET_REFLECTION_BSDF_H
#define MICROFACET_REFLECTION_BSDF_H

#include <bsdf.h>

namespace manta {

	class MicrofacetDistribution;
	class MediaInterface;

	class MicrofacetReflectionBSDF : public BSDF {
	public:
		MicrofacetReflectionBSDF();
		~MicrofacetReflectionBSDF();

		virtual void initializeSessionMemory(const IntersectionPoint *surfaceInteraction, NodeSessionMemory *memory, StackAllocator *stackAllocator) const;

		virtual math::Vector sampleF(const IntersectionPoint *surfaceInteraction, const math::Vector &i, math::Vector *o, math::real *pdf, StackAllocator *stackAllocator) const;
		virtual math::real calculatePDF(const IntersectionPoint *surfaceInteraction, const math::Vector &i, const math::Vector &o, StackAllocator *stackAllocator) const;

		void setDistribution(MicrofacetDistribution *distribution) { m_distribution = distribution; }
		MicrofacetDistribution *getDistribution() const { return m_distribution; }

	protected:
		MicrofacetDistribution *m_distribution;
	};

} /* namespace manta */

#endif /* MICROFACET_REFLECTION_BSDF_H */
