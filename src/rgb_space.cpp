#include <rgb_space.h>

const manta::RgbXyzMatrix manta::RgbSpace::CieXyzToSrgb = {
	{ 3.2406, -1.5372 , -0.4986 },
	{ -0.9689,  1.8758,  0.0415 },
	{ 0.0557, -0.2040,  1.0570 }
};

const manta::RgbXyzMatrix &manta::RgbSpace::getRgbMatrix(WorkingSpace space) {
	switch (space) {
	case SRGB:
		return CieXyzToSrgb;
	default:
		return CieXyzToSrgb;
	}
}

manta::ColorRgb manta::RgbSpace::convertToSrgb(const ColorXyz &cieXyz) {
	const RgbXyzMatrix &cm = CieXyzToSrgb;

	// Scale to D65 standard illuminant
	ColorRgb scaledXyz = cieXyz;

	ColorRgb intermediate = ColorRgb(
		scaledXyz.x * cm.data[0][0] + scaledXyz.y * cm.data[0][1] + scaledXyz.z * cm.data[0][2],
		scaledXyz.x * cm.data[1][0] + scaledXyz.y * cm.data[1][1] + scaledXyz.z * cm.data[1][2],
		scaledXyz.x * cm.data[2][0] + scaledXyz.y * cm.data[2][1] + scaledXyz.z * cm.data[2][2]
	);

	// Adjust gamma
	intermediate.x = clip(adjustGammaSrgb(intermediate.x));
	intermediate.y = clip(adjustGammaSrgb(intermediate.y));
	intermediate.z = clip(adjustGammaSrgb(intermediate.z));

	return intermediate;
}

manta::math::real_d manta::RgbSpace::adjustGammaSrgb(math::real_d u) {
	constexpr math::real_d MIN_SRGB_POWER_POINT = (math::real_d)0.0031308;

	if (u < MIN_SRGB_POWER_POINT) {
		return 12.92 * u;
	}
	else {
		return 1.055 * pow(u, 1 / 2.4) - 0.055;
	}
}

manta::math::real_d manta::RgbSpace::clip(math::real_d u) {
	if (u < (math::real_d)0.0) return (math::real_d)0.0;
	else if (u > (math::real_d)1.0) return (math::real_d)1.0;
	else return u;
}
