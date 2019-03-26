#ifndef SQUARE_APERTURE_H
#define SQUARE_APERTURE_H

#include <aperture.h>

namespace manta {

	class SquareAperture : public Aperture {
	public:
		SquareAperture();
		~SquareAperture();

		virtual bool filter(math::real x, math::real y) const;
	};

} /* namespace manta */

#endif /* SQUARE_APERTURE_H */