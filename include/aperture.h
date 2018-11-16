#ifndef APERTURE_H
#define APERTURE_H

#include <manta_math.h>

namespace manta {

	class Aperture {
	public:
		Aperture() {}
		~Aperture() {}

		virtual bool filter(math::real x, math::real y) const = 0;
		virtual void setSize(math::real size) = 0;
	};

} /* namespace manta */

#endif /* APERTURE_H */
