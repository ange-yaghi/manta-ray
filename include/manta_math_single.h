#ifndef MANTA_MATH_SINGLE_H
#define MANTA_MATH_SINGLE_H

#include <manta_real.h>

#include <math.h>
#include <random>

#define MATH_CONST extern const __declspec(selectany)

namespace manta {

	namespace math {

		// Vector types
		struct Generic {
			union {
				struct {
					real x, y, z, w;
				};
				struct {
					real qw, qx, qy, qz;
				};
			};
		};

		typedef Generic Vector;
		typedef Generic Quaternion;

	} /* namespace math */

} /* namespace manta */

#endif /* MANTA_MATH_SINGLE_H */
