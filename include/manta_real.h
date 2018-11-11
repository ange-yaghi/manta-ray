#ifndef MANTA_REAL_H
#define MANTA_REAL_H

#include <manta_math_conf.h>

#include <float.h>

// Define the real parameter based on the configured parameter

namespace manta {
	namespace math {

#if MANTA_PRECISION == MANTA_PRECISION_FLOAT
		typedef float real;
		constexpr real REAL_MAX = FLT_MAX;
#else
		typedef double real;
		constexpr real REAL_MAX = DBL_MAX;
#endif /* MANTA_PRECISION */
	}
}

#define R (real)

#endif /* MANTA_REAL_H */
