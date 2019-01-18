#ifndef BSDF_H
#define BSDF_H

#include <manta_math.h>

namespace manta {

	class BSDF {
	public:
		BSDF();
		~BSDF();

		virtual math::Vector generateMicrosurfaceNormal(const math::Vector &normal, const math::Vector &incident, const math::Vector &u, const math::Vector &v) const = 0;
		virtual math::real generateWeight(const math::Vector &normal, const math::Vector &incident, const math::Vector &m, const math::Vector &o) const = 0;
		virtual math::real bidirectionalShadowMasking(const math::Vector &normal, const math::Vector &incident, const math::Vector &o, const math::Vector &m) const;

	protected:
		math::real smithBidirectionalShadowMasking(const math::Vector &normal, const math::Vector &incident, const math::Vector &o, const math::Vector &m) const;
		virtual math::real g1(const math::Vector &normal, const math::Vector &incident, const math::Vector &v, const math::Vector &m) const = 0;
	};

} /* namespace manta */

#endif /* BSDF */
