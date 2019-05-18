#include <rgb_space.h>

#include <algorithm>

const manta::RgbSpace manta::RgbSpace::srgb = {
	{(math::real_d)0.64, (math::real_d)0.3, (math::real_d)0.15}, // x
	{(math::real_d)0.33, (math::real_d)0.6, (math::real_d)0.06}, // y
	{(math::real_d)0.3127, (math::real_d)0.3290} // White point
};

manta::RgbSpace::RgbSpace() {
	/* void */
}

manta::RgbSpace::RgbSpace(
		const math::Vector3_d &x, const math::Vector3_d &y, const math::Vector2_d &whitePoint){
	m_x = x;
	m_y = y; 
	m_whitePoint = whitePoint;
}

manta::RgbSpace::~RgbSpace() {
	/* void */
}

manta::math::real_d manta::RgbSpace::applyGammaSrgb(math::real_d u) {
	static constexpr math::real_d MIN_SRGB_POWER_POINT = (math::real_d)0.0031308;

	if (u < MIN_SRGB_POWER_POINT) {
		return (math::real_d)12.92 * u;
	}
	else {
		return (math::real_d)1.055 * ::pow(u, 1 / (math::real_d)2.4) - (math::real_d)0.055;
	}
}

manta::math::real_d manta::RgbSpace::inverseGammaSrgb(math::real_d u) {
	static constexpr math::real_d MIN_SRGB_POWER_POINT = (math::real_d)0.04045;

	if (u < MIN_SRGB_POWER_POINT) {
		return u / (math::real_d)12.92;
	}
	else {
		return ::pow((u + (math::real_d)0.055) / (math::real_d)1.055, (math::real_d)2.4);
	}
}

manta::math::real_d manta::RgbSpace::clip(math::real_d u) {
	if (u < (math::real_d)0.0) return (math::real_d)0.0;
	else if (u > (math::real_d)1.0) return (math::real_d)1.0;
	else return u;
}

manta::ColorRgb manta::RgbSpace::convertToRgb(const ColorXyy &col) const {
	// Calculate implied z-values
	math::real_d zr = 1 - (m_x.r + m_y.r);
	math::real_d zg = 1 - (m_x.g + m_y.g);
	math::real_d zb = 1 - (m_x.b + m_y.b);
	math::real_d zw = 1 - (m_whitePoint.x + m_whitePoint.y);

	// Compute the XYZ to RGB matrix
	math::real_d rx = (m_y.g * zb) - (m_y.b * zg);
	math::real_d ry = (m_x.b * zg) - (m_x.g * zb);
	math::real_d rz = (m_x.g * m_y.b) - (m_x.b * m_y.g);
	math::real_d gx = (m_y.b * zr) - (m_y.r * zb);
	math::real_d gy = (m_x.r * zb) - (m_x.b * zr);
	math::real_d gz = (m_x.b * m_y.r) - (m_x.r * m_y.b);
	math::real_d bx = (m_y.r * zg) - (m_y.g * zr);
	math::real_d by = (m_x.g * zr) - (m_x.r * zg);
	math::real_d bz = (m_x.r * m_y.g) - (m_x.g * m_y.r);

	// Compute the RGB luminance scaling factor
	math::real_d rw = ((rx * m_whitePoint.x) + (ry * m_whitePoint.y) + 
		(rz * zw)) / m_whitePoint.y;
	math::real_d gw = ((gx * m_whitePoint.x) + (gy * m_whitePoint.y) + 
		(gz * zw)) / m_whitePoint.y;
	math::real_d bw = ((bx * m_whitePoint.x) + (by * m_whitePoint.y) + 
		(bz * zw)) / m_whitePoint.y;

	// Scale the XYZ to RGB matrix to white
	rx = rx / rw;  ry = ry / rw;  rz = rz / rw;
	gx = gx / gw;  gy = gy / gw;  gz = gz / gw;
	bx = bx / bw;  by = by / bw;  bz = bz / bw;

	// Calculate the desired RGB
	ColorRgb output;
	output.r = (rx * col.x) + (ry * col.y) + (rz * col.z);
	output.g = (gx * col.x) + (gy * col.y) + (gz * col.z);
	output.b = (bx * col.x) + (by * col.y) + (bz * col.z);

	// Constrain the RGB color within the RGB gamut
	output.r = clip(output.r);
	output.g = clip(output.g);
	output.b = clip(output.b);

	return output;
}

manta::ColorXyy manta::RgbSpace::xyzToXyy(const ColorXyz &col) {
	ColorXyy output;

	math::real_d div = 1 / (col.x + col.y + col.z);
	output.x = col.x * div;
	output.y = col.y * div;
	output.z = 1 - (output.x + output.y);

	return output;
}
