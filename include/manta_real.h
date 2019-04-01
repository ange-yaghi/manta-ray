#ifndef MANTA_REAL_H
#define MANTA_REAL_H

#include <manta_math_conf.h>

#include <float.h>

// Define the real parameter based on the configured parameter

namespace manta {

	namespace math {

		// Standard definitions
		typedef float real_f;
		typedef double real_d;

#if MANTA_PRECISION == MANTA_PRECISION_FLOAT

		typedef real_f real;

		namespace constants {

			constexpr real REAL_MAX = FLT_MAX;

		} /* namespace constants */

#else /* MANTA_PRECISION == MANTA_PRECISION_DOUBLE */

		typedef real_d real;

		namespace constants {

			constexpr real REAL_MAX = DBL_MAX;

		} /* namespace constants */

#endif /* MANTA_PRECISION */

	} /* namespace math */

} /* namespace manta */

#endif /* MANTA_REAL_H */
