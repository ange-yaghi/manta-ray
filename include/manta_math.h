#ifndef MANTA_MATH_H
#define MANTA_MATH_H

#include <manta_math_conf.h>

#include <limits>

#if MANTA_USE_SIMD == true

#if MANTA_PRECISION == MANTA_PRECISION_FLOAT
#include <manta_math_float_simd.h>

#else /* MANTA_PRECISION_DOUBLE */
#error "Double precision SIMD implementation is not defined"
#endif /* MANTA_PRECISION */

#else /* MANTA_USE_SIMD == false */

#include <manta_math_single.h>

#endif /* MANTA_USE SIMD */

// Macros
#define MATH_CONST extern const __declspec(selectany)

// Standard math functions and definitions
namespace manta {
	namespace math {

		struct VectorMask {
			union {
				struct {
					int mask[4];
				};

				Vector vector;
			};

			operator Vector() const { return vector; }
		};

		struct Matrix {
			Vector rows[4];
		};


		// Storage Data Types

		struct Vector2 {
			Vector2() : x(0), y(0) {}
			Vector2(float x, float y) : x(x), y(y) {}

			union {
				struct {
					float x, y;
				};

				float vec[2];
			};
		};

		struct Vector3 {
			Vector3() : x(0), y(0), z(0) {}
			Vector3(float x, float y, float z) : x(x), y(y), z(z) {}
			Vector3(const Vector2 &v) : x(v.x), y(v.y), z(0) {}

			union {
				struct {
					float x, y, z;
				};

				float vec[3];
			};
		};

		struct Vector4 {
			Vector4() : x(0), y(0), z(0), w(0) {}
			Vector4(float x, float y, float z, float w = 0) : x(x), y(y), z(z), w(w) {}

			Vector4(const Vector3 &v) : x(v.x), y(v.y), z(v.z), w(0) {}

			void set(float x, float y, float z, float w) { this->x = x; this->y = y; this->z = z; this->w = w; }

			union {
				struct {
					float x, y, z, w;
				};

				float vec[4];
			};
		};

		struct Matrix44 {
			Matrix44() {}
			~Matrix44() {}

			union {
				struct {
					Vector4 rows[4];
				};

				float data[4][4];
			};
		};

		struct Matrix33 {
			Matrix33() {}
			~Matrix33() {}

			union {
				struct {
					Vector3 rows[3];
				};

				float data[3][3];
			};
		};

		namespace constants {
			// Masks
			MATH_CONST VectorMask MaskOffW = { (int)0xFFFFFFFF,(int)0xFFFFFFFF,(int)0xFFFFFFFF, (int)0x00000000 };
			MATH_CONST VectorMask MaskOffZ = { (int)0xFFFFFFFF, (int)0xFFFFFFFF, (int)0x00000000, (int)0xFFFFFFFF };
			MATH_CONST VectorMask MaskOffY = { (int)0xFFFFFFFF, (int)0x00000000, (int)0xFFFFFFFF, (int)0xFFFFFFFF };
			MATH_CONST VectorMask MaskOffX = { (int)0x00000000, (int)0xFFFFFFFF, (int)0xFFFFFFFF, (int)0xFFFFFFFF };

			MATH_CONST VectorMask MaskKeepW = { (int)0x00000000, (int)0x00000000, (int)0x00000000, (int)0xFFFFFFFF };
			MATH_CONST VectorMask MaskKeepZ = { (int)0x00000000, (int)0x00000000, (int)0xFFFFFFFF, (int)0x00000000 };
			MATH_CONST VectorMask MaskKeepY = { (int)0x00000000, (int)0xFFFFFFFF, (int)0x00000000, (int)0x00000000 };
			MATH_CONST VectorMask MaskKeepX = { (int)0xFFFFFFFF, (int)0x00000000, (int)0x00000000, (int)0x00000000 };

			// Axes
			MATH_CONST Vector XAxis = { (real)1.0, (real)0.0, (real)0.0, (real)0.0 };
			MATH_CONST Vector YAxis = { (real)0.0, (real)1.0, (real)0.0, (real)0.0 };
			MATH_CONST Vector ZAxis = { (real)0.0, (real)0.0, (real)1.0, (real)0.0 };

			// Constants
			MATH_CONST Vector IdentityRow1 = { (real)1.0, (real)0.0, (real)0.0, (real)0.0 };
			MATH_CONST Vector IdentityRow2 = { (real)0.0, (real)1.0, (real)0.0, (real)0.0 };
			MATH_CONST Vector IdentityRow3 = { (real)0.0, (real)0.0, (real)1.0, (real)0.0 };
			MATH_CONST Vector IdentityRow4 = { (real)0.0, (real)0.0, (real)0.0, (real)1.0 };

			MATH_CONST Vector Negate = { (real)-1.0, (real)-1.0, (real)-1.0, (real)-1.0 };
			MATH_CONST Vector Negate3 = { (real)-1.0, (real)-1.0, (real)-1.0, (real)1.0 };
			MATH_CONST Vector One = { (real)1.0, (real)1.0, (real)1.0, (real)1.0 };
			MATH_CONST Vector Zero = { (real)0.0, (real)0.0, (real)0.0, (real)0.0 };
			MATH_CONST Vector Zero3 = { (real)0.0, (real)0.0, (real)0.0, (real)1.0 };
			MATH_CONST Vector Half = { (real)0.5, (real)0.5, (real)0.5, (real)0.5 };
			MATH_CONST Vector Double = { (real)2.0, (real)2.0, (real)2.0, (real)2.0 };

			// Numeral Constants
			MATH_CONST real PI = (real)3.141592654;
			MATH_CONST real TWO_PI = (real)6.2831853071795864769252866;
			MATH_CONST real SQRT_2 = (real)1.41421356237;

			// Quaternions
			MATH_CONST Quaternion QuatIdentity = { (real)1.0, (real)0.0, (real)0.0, (real)0.0 };

		} /* namespace constants */

		// Math functions
		Vector uniformRandom4(real range = (real)1.0);
		real uniformRandom(real range = (real)1.0);

		// Vector/General Quaternion
		Generic loadScalar(real s);
		Generic loadVector(real x = (real)0.0, real y = (real)0.0, real z = (real)0.0, real w = (real)0.0);
		Generic loadVector(const Vector4 &v);
		Generic loadVector(const Vector3 &v, real w = (real)0.0);
		Generic loadVector(const Vector2 &v1);
		Generic loadVector(const Vector2 &v1, const Vector2 &v2);
		Quaternion loadQuaternion(real angle, const Vector &axis);

		Generic componentMax(const Generic &a, const Generic &b);
		Generic componentMin(const Generic &a, const Generic &b);

		Vector4 getVector4(const Vector &v);
		Vector3 getVector3(const Vector &v);
		Vector2 getVector2(const Vector &v);
		real getScalar(const Vector &v);

		real getX(const Vector &v);
		real getY(const Vector &v);
		real getZ(const Vector &v);
		real getW(const Vector &v);

		void setX(Vector &v, math::real value);
		void setY(Vector &v, math::real value);
		void setZ(Vector &v, math::real value);
		void setW(Vector &v, math::real value);
		void set(Vector &v, int index, math::real value);

		inline real realNAN() { return std::numeric_limits<real>::quiet_NaN(); }
		real get(const Vector &v, int index);

		real getQuatX(const Quaternion &v);
		real getQuatY(const Quaternion &v);
		real getQuatZ(const Quaternion &v);
		real getQuatW(const Quaternion &v);

		Generic add(const Generic &v1, const Generic &v2);
		Generic sub(const Generic &v1, const Generic &v2);
		Generic mul(const Generic &v1, const Generic &v2);
		Generic div(const Generic &v1, const Generic &v2);

		Vector abs(const Vector &v);
		Vector dot(const Vector &v1, const Vector &v2);
		Vector dot3(const Vector &v1, const Vector &v2);
		Vector cross(const Vector &v1, const Vector &v2);
		Vector sqrt(const Vector &v);
		Vector magnitudeSquared3(const Vector &v);
		Vector magnitude(const Vector &v);
		Vector normalize(const Vector &v);
		Vector negate(const Vector &v);
		Vector negate3(const Vector &v);

		Generic permute(const Generic &v, int kx, int ky, int kz, int kw = 3);
		int maxDimension(const Generic &v);
		int maxDimension3(const Generic &v);

		Vector mask(const Vector &v, const VectorMask &mask);
		Vector bitOr(const Vector &v1, const Vector &v2);

		Vector reflect(const Vector &v, const Vector &n);
		Vector inverseReflect(const Vector &v, const Vector &r);

		// Quaternion
		Quaternion quatInvert(const Quaternion &q);
		Quaternion quatMultiply(const Quaternion &q1, const Quaternion &q2);
		Vector quatTransform(const Quaternion &q, const Vector &v);
		Quaternion quatAddScaled(const Quaternion &q, const Vector &vec, real scale);

		// Matrices
		Matrix loadIdentity();
		Matrix loadMatrix(const Vector &r1, const Vector &r2, const Vector &r3, const Vector &r4);
		Matrix loadMatrix(const Quaternion &quat);
		Matrix loadMatrix(const Quaternion &quat, const Vector &origin);
		void loadMatrix(const Quaternion &quat, const Vector &origin, Matrix *full, Matrix *orientation);

		Matrix transpose(const Matrix &m);
		Matrix orthogonalInverse(const Matrix &m);

		Matrix44 getMatrix44(const Matrix &m);
		Matrix33 getMatrix33(const Matrix &m);

		Vector extendVector(const Vector &v);
		Vector matMult(const Matrix &m, const Vector &v);
		Matrix matMult(const Matrix &m1, const Matrix &m2);

		// Common Matrix Calculations
		Matrix frustrumPerspective(float fovy, float aspect, float near, float far);
		Matrix orthographicProjection(float width, float height, float near, float far);
		Matrix cameraTarget(const Vector &eye, const Vector &target, const Vector &up);

		Matrix translationTransform(const Vector &translation);
		Matrix scaleTransform(const Vector &scale);
		Matrix rotationTransform(const Vector &axis, float angle);

		Vector getTranslationPart(const Matrix &mat);

		real clamp(real value);

	} /* namespace math */

} /* namespace manta */

#endif /* MANTA_MATH_H */
