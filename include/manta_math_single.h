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

		namespace constants {
			MATH_CONST Vector XAxis = { (real)1.0, (real)0.0, (real)0.0, (real)0.0 };
			MATH_CONST Vector YAxis = { (real)0.0, (real)1.0, (real)0.0, (real)0.0 };
			MATH_CONST Vector ZAxis = { (real)0.0, (real)0.0, (real)1.0, (real)0.0 };

			MATH_CONST Vector Negate = { (real)-1.0, (real)-1.0, (real)-1.0, (real)-1.0 };
			MATH_CONST Vector Negate3 = { (real)-1.0, (real)-1.0, (real)-1.0, (real)1.0 };
			MATH_CONST Vector One = { (real)1.0, (real)1.0, (real)1.0, (real)1.0 };
			MATH_CONST Vector Zero = { (real)0.0, (real)0.0, (real)0.0, (real)0.0 };
			MATH_CONST Vector Zero3 = { (real)0.0, (real)0.0, (real)0.0, (real)1.0 };
			MATH_CONST Vector Half = { (real)0.5, (real)0.5, (real)0.5, (real)0.5 };
			MATH_CONST Vector Double = { (real)2.0, (real)2.0, (real)2.0, (real)2.0 };

			MATH_CONST real PI = (real)3.141592654;
			MATH_CONST real TWO_PI = (real)6.2831853071795864769252866;
			MATH_CONST real SQRT_2 = (real)1.41421356237f;
		}

		Generic loadScalar(real s = (real)0.0);
		Generic loadVector(real x = (math::real)0.0, real y = (real)0.0, real z = (real)0.0, real w = (real)0.0);

		real getX(const Vector &v);
		real getY(const Vector &v);
		real getZ(const Vector &v);
		real getW(const Vector &v);

		real getQuatX(const Quaternion &q);
		real getQuatY(const Quaternion &q);
		real getQuatZ(const Quaternion &q);
		real getQuatW(const Quaternion &q);

		real getScalar(const Vector &v);

		Generic add(const Generic &v1, const Generic &v2);
		Generic sub(const Generic &v1, const Generic &v2);
		Generic mul(const Generic &v1, const Generic &v2);
		Generic div(const Generic &v1, const Generic &v2);

		Vector dot(const Vector &v1, const Vector &v2);
		Vector dot3(const Vector &v1, const Vector &v2);

		Vector cross(const Vector &v1, const Vector &v2);

		Vector sqrt(const Vector &v);

		Vector magnitudeSquared3(const Vector &v);
		Vector magnitude(const Vector &v);
		Vector normalize(const Vector &v);

		Vector negate(const Vector &v);
		Vector negate3(const Vector &v);
		Vector quatInvert(const Quaternion &q);
		Vector quatMultiply(const Quaternion &q1, const Quaternion &q2);

		Vector uniformRandom4(real range = (real)1.0);
		real uniformRandom(real range = (real)1.0);

		real clamp(real value);
	}
}

#endif /* MANTA_MATH_SINGLE_H */
