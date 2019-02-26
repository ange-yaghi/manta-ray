#ifndef MICROFACET_GLASS_BSDF_H
#define MICROFACET_GLASS_BSDF_H

#include <bsdf.h>

namespace manta {

	class MicrofacetDistribution;
	class MediaInterface;

	class MicrofacetGlassBSDF : public BSDF {
	public:
		MicrofacetGlassBSDF();
		~MicrofacetGlassBSDF();

		virtual void initialize(const IntersectionPoint *surfaceInteraction, MaterialNodeMemory *memory, StackAllocator *stackAllocator) const;

		virtual math::Vector sampleF(const IntersectionPoint *surfaceInteraction, const math::Vector &i, math::Vector *o, math::real *pdf, StackAllocator *stackAllocator) const;
		virtual math::real calculatePDF(const IntersectionPoint *surfaceInteraction, const math::Vector &i, const math::Vector &o, StackAllocator *stackAllocator) const;

		void setDistribution(MicrofacetDistribution *distribution) { m_distribution = distribution; }
		MicrofacetDistribution *getDistribution() const { return m_distribution; }

		void setMediaInterface(MediaInterface *mediaInterface) { m_mediaInterface = mediaInterface; }
		MediaInterface *getMediaInterface() { return m_mediaInterface; }

	protected:
		MicrofacetDistribution *m_distribution;
		MediaInterface *m_mediaInterface;
	};

} /* namespace manta */

#endif /* MICROFACET_GLASS_BSDF_H */

