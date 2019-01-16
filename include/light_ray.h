#ifndef LIGHT_RAY_H
#define LIGHT_RAY_H

#include <manta_math.h>

namespace manta {

	class LightRay {
	public:
		LightRay();
		~LightRay();

		math::Vector getDirection() const { return m_direction; }
		void setDirection(const math::Vector &dir) { m_direction = dir; }

		math::Vector getSource() const { return m_source; }
		void setSource(const math::Vector &source) { m_source = source; }

		math::Vector getIntensity() const { return m_intensity; }
		void setIntensity(const math::Vector &intensity) { m_intensity = intensity; }

		math::Vector getWeightedIntensity() const { return math::mul(m_intensity, math::loadScalar(m_weight)); }

		void setWeight(math::real weight) { m_weight = weight; }
		math::real getWeight() const { return m_weight; }

	protected:
		math::Vector m_direction;
		math::Vector m_source;

		math::Vector m_intensity;
		math::real m_weight;
	};

} /* namespace manta */

#endif /* LIGHT_RAY_H */
