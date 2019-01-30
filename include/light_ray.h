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

		void calculateTransformations();

		int getKX() const { return m_kx; }
		int getKY() const { return m_ky; }
		int getKZ() const { return m_kz; }
		const math::Vector3 &getShear() const { return m_shear; }
		math::Vector getPermutedDirection() const { return m_permutedDirection; }
		math::Vector getInverseDirection() const { return m_inverseDirection; }

		void setMeta(int meta) { m_meta = meta; }
		int getMeta() const { return m_meta; }

	protected:
		int m_meta;

		math::Vector m_direction;
		math::Vector m_source;

		math::Vector m_intensity;
		math::real m_weight;

		int m_kx, m_ky, m_kz;
		math::Vector3 m_shear;
		math::Vector m_permutedDirection;
		math::Vector m_inverseDirection;
	};

} /* namespace manta */

#endif /* LIGHT_RAY_H */
