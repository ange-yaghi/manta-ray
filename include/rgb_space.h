#ifndef RGB_SPACE_H
#define RGB_SPACE_H

#include <color.h>
#include <manta_math.h>

namespace manta {

	class RgbSpace {
	public:
		static const RgbSpace srgb;

	public:
		RgbSpace() {}
		RgbSpace(const math::Vector3_d &x, const math::Vector3_d &y, const math::Vector2_d whitePoint) { m_x = x; m_y = y; m_whitePoint = whitePoint; }
		~RgbSpace() {}

		static math::real_d applyGammaSrgb(math::real_d u);
		static math::real_d inverseGammaSrgb(math::real_d u);

		static math::real_d clip(math::real_d u);

		void setX(const math::Vector3_d &x) { m_x = x; }
		math::Vector3_d getX() const { return m_x; }

		void setY(const math::Vector3_d &y) { m_y = y; }
		math::Vector3_d getY() const { return m_y; }

		void setWhitePoint(const math::Vector2_d &w) { m_whitePoint = w; }
		math::Vector2_d getWhitePoint() const { return m_whitePoint; }

		ColorRgb convertToRgb(const ColorXyy &col) const;

		static ColorXyy xyzToXyy(const ColorXyz &col);

	protected:
		math::Vector3_d m_x;
		math::Vector3_d m_y;
		math::Vector2_d m_whitePoint;
	};

} /* namespace manta */

#endif /* RGB_SPACE_H */
