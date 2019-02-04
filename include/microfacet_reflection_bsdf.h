#ifndef MICROFACET_REFLECTION_BSDF_H
#define MICROFACET_REFLECTION_BSDF_H

#include <bsdf.h>

namespace manta {

	class MicrofacetDistribution;

	class MicrofacetReflectionBSDF : public BSDF {
	public:
		MicrofacetReflectionBSDF();
		~MicrofacetReflectionBSDF();

		virtual math::Vector sampleF(const math::Vector &i, math::Vector *o, math::real *pdf) const;

		void setDistribution(MicrofacetDistribution *distribution) { m_distribution = distribution; }
		MicrofacetDistribution *getDistribution() const { return m_distribution; }

		MicrofacetDistribution *m_distribution;
	};

} /* namespace manta */

#endif /* MICROFACET_REFLECTION_BSDF_H */
