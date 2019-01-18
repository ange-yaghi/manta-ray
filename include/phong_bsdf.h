#ifndef PHONG_BSDF_H
#define PHONG_BSDF_H

#include <bsdf.h>

namespace manta {

	class PhongBSDF : public BSDF {
	public:
		PhongBSDF();
		~PhongBSDF();

		virtual math::Vector generateMicrosurfaceNormal(const math::Vector &normal, const math::Vector &incident, const math::Vector &u, const math::Vector &v) const;
		virtual math::real generateWeight(const math::Vector &n, const math::Vector &i, const math::Vector &m, const math::Vector &o) const;

		void setPower(math::real power) { m_power = power; }
		math::real getPower() const { return m_power; }

	protected:
		virtual math::real g1(const math::Vector &n, const math::Vector &i, const math::Vector &v, const math::Vector &m) const;

		math::real m_power;
	};

} /* namespace manta */

#endif /* PHONG_BSDF_H */
