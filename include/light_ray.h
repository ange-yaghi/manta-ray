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

	protected:
		math::Vector m_direction;
		math::Vector m_source;

		math::Vector m_intensity;
	};

} /* namespace manta */

#endif /* LIGHT_RAY_H */
