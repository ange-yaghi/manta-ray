#ifndef LAMBERTIAN_BSDF_H
#define LAMBERTIAN_BSDF_H

#include <bsdf.h>

namespace manta {

	class LambertianBSDF : public BSDF {
	public:
		LambertianBSDF();
		~LambertianBSDF();

		virtual math::Vector generateMicrosurfaceNormal(const math::Vector &normal, const math::Vector &incident, const math::Vector &u, const math::Vector &v) const;
		virtual math::real generateWeight(const math::Vector &normal, const math::Vector &incident, const math::Vector &m, const math::Vector &o) const;
		virtual math::real bidirectionalShadowMasking(const math::Vector &normal, const math::Vector &incident, const math::Vector &o, const math::Vector &m) const;

	protected:
		virtual math::real g1(const math::Vector &normal, const math::Vector &incident, const math::Vector &v, const math::Vector &m) const;
	};

} /* namespace manta */

#endif /* LAMBERTIAN_BSDF_H */

