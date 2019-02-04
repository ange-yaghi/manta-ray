#ifndef MICROFACET_DISTRIBUTION_H
#define MICROFACET_DISTRIBUTION_H

#include <manta_math.h>

namespace manta {

	class MicrofacetDistribution {
	public:
		MicrofacetDistribution();
		~MicrofacetDistribution();

		virtual math::Vector generateMicrosurfaceNormal() const = 0;
		virtual math::real calculatePDF(const math::Vector &o, const math::Vector &m) const;
		virtual math::real calculateDistribution(const math::Vector &m) const = 0;
		virtual math::real calculateG1(const math::Vector &v, const math::Vector &m) const = 0;
		virtual math::real bidirectionalShadowMasking(const math::Vector &i, const math::Vector &o, const math::Vector &m) const;

		math::real smithBidirectionalShadowMasking(const math::Vector &i, const math::Vector &o, const math::Vector &m) const;
	};

} /* namespace manta */

#endif /* MICROFACET_DISTRIBUTION_H */
