#ifndef MANTA_MATH_H
#define MANTA_MATH_H

#include <manta_math_conf.h>

#if MANTA_USE_SIMD == true

#if MANTA_PRECISION == MANTA_PRECISION_FLOAT
#include <manta_math_float_simd.h>

#else /* MANTA_PRECISION_DOUBLE */
#error "Double precision SIMD implementation is not defined"
#endif /* MANTA_PRECISION */

#else /* MANTA_USE_SIMD == false */

#include <manta_math_single.h>

#endif /* MANTA_USE SIMD */

#endif /* MANTA_MATH_H */
