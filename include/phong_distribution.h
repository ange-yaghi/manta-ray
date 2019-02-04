#ifndef PHONG_DISTRIBUTION_H
#define PHONG_DISTRIBUTION_H

#include <microfacet_distribution.h>

namespace manta {

	class PhongDistribution : public MicrofacetDistribution {
	public:
		PhongDistribution();
		~PhongDistribution();

		virtual math::Vector generateMicrosurfaceNormal() const;
		virtual math::real calculateDistribution(const math::Vector &m) const;
		virtual math::real calculateG1(const math::Vector &v, const math::Vector &m) const;

		void setPower(math::real power) { m_power = power; }
		math::real getPower() const { return m_power; }

	protected:
		math::real m_power;
	};

} /* namespace manta */

#endif /* PHONG_DISTRIBUTION_H */
