#ifndef CIRCULAR_APERTURE_H
#define CIRCULAR_APERTURE_H

#include <aperture.h>

namespace manta {

	class CircularAperture : public Aperture {
	public:
		CircularAperture();
		virtual ~CircularAperture();

		virtual bool filter(math::real x, math::real y) const;
	};

} /* namespace manta */

#endif /* CIRCULAR_APERTURE_H */
