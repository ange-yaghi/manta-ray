#ifndef MANTA_MATH_FLOAT_SIMD_H
#define MANTA_MATH_FLOAT_SIMD_H

#include <xmmintrin.h>
#include <stdlib.h>

#include <manta_real.h>

// Extra Definitions

#define _mm_replicate_x_ps(v) \
	_mm_shuffle_ps((v), (v), _MM_SHUFFLE(0, 0, 0, 0))

#define _mm_replicate_y_ps(v) \
	_mm_shuffle_ps((v), (v), _MM_SHUFFLE(1, 1, 1, 1))

#define _mm_replicate_z_ps(v) \
	_mm_shuffle_ps((v), (v), _MM_SHUFFLE(2, 2, 2, 2))

#define _mm_replicate_w_ps(v) \
	_mm_shuffle_ps((v), (v), _MM_SHUFFLE(3, 3, 3, 3))

#define _mm_madd_ps(a, b, c) \
	_mm_add_ps(_mm_mul_ps((a), (b)), (c));

namespace manta {
	namespace math {

		// Main Arithmetic Data Types
		typedef __m128 Vector;
		typedef __m128 Quaternion;
		typedef __m128 Generic;

	} /* namespace math */
} /* namespace manta */

#endif /* MANTA_MATH_FLOAT_SIMD_H */
