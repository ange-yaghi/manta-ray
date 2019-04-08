#ifndef RGB_SPACE_H
#define RGB_SPACE_H

#include <color.h>
#include <manta_math.h>

namespace manta {

	typedef math::Matrix33_t<math::real_d> RgbXyzMatrix;

	class RgbSpace {
	public:
		static const RgbXyzMatrix CieXyzToSrgb;

	public:
		enum WorkingSpace {
			SRGB
		};

	public:
		static const RgbXyzMatrix &getRgbMatrix(WorkingSpace space);

		static ColorRgb convertToSrgb(const ColorXyz &cieXyz);
		static math::real_d adjustGammaSrgb(math::real_d u);

		static math::real_d clip(math::real_d u);
	};

} /* namespace manta */

#endif /* RGB_SPACE_H */
