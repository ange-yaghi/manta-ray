#ifndef MANTA_MATH_H
#define MANTA_MATH_H

#include <xmmintrin.h>
#include <stdlib.h>

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

#define MATH_CONST extern const __declspec(selectany)

		namespace constants {

			// ----------------------------------------------------
			// Constants
			// ----------------------------------------------------

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
			MATH_CONST Vector XAxis = { 1.0f, 0.0f, 0.0f, 0.0f };
			MATH_CONST Vector YAxis = { 0.0f, 1.0f, 0.0f, 0.0f };
			MATH_CONST Vector ZAxis = { 0.0f, 0.0f, 1.0f, 0.0f };

			// Constants
			MATH_CONST Vector IdentityRow1 = { 1.0f, 0.0f, 0.0f, 0.0f };
			MATH_CONST Vector IdentityRow2 = { 0.0f, 1.0f, 0.0f, 0.0f };
			MATH_CONST Vector IdentityRow3 = { 0.0f, 0.0f, 1.0f, 0.0f };
			MATH_CONST Vector IdentityRow4 = { 0.0f, 0.0f, 0.0f, 1.0f };

			MATH_CONST Vector Negate = { -1.0f, -1.0f, -1.0f, -1.0f };
			MATH_CONST Vector Negate3 = { -1.0f, -1.0f, -1.0f, 1.0f };
			MATH_CONST Vector One = { 1.0f, 1.0f, 1.0f, 1.0f };
			MATH_CONST Vector Zero = { 0.0f, 0.0f, 0.0f, 0.0f };
			MATH_CONST Vector Zero3 = { 0.0f, 0.0f, 0.0f, 1.0f };
			MATH_CONST Vector Half = { 0.5f, 0.5f, 0.5f, 0.5f };
			MATH_CONST Vector Double = { 2.0f, 2.0f, 2.0f, 2.0f };

			// Numeral Constants
			MATH_CONST float PI = 3.141592654f;
			MATH_CONST float TWO_PI = 6.2831853071795864769252866f;
			MATH_CONST float SQRT_2 = 1.41421356237f;

			// Quaternions
			MATH_CONST Quaternion QuatIdentity = { 1.0f, 0.0f, 0.0f, 0.0f };

		}

		// ----------------------------------------------------
		// Math Functions
		// ----------------------------------------------------

		Vector uniformRandom4(float range = 1.0f);
		float uniformRandom(float range = 1.0f);

		// Vector/General Quaternion
		Generic loadScalar(float s);
		Generic loadVector(float x = 0.0f, float y = 0.0f, float z = 0.0f, float w = 0.0f);
		Generic loadVector(const Vector4 &v);
		Generic loadVector(const Vector3 &v, float w = 0.0f);
		Generic loadVector(const Vector2 &v1);
		Generic loadVector(const Vector2 &v1, const Vector2 &v2);
		Quaternion loadQuaternion(float angle, const Vector &axis);

		Vector4 getVector4(const Vector &v);
		Vector3 getVector3(const Vector &v);
		Vector2 getVector2(const Vector &v);
		float getScalar(const Vector &v);

		float getX(const Vector &v);
		float getY(const Vector &v);
		float getZ(const Vector &v);
		float getW(const Vector &v);

		float getQuatX(const Quaternion &v);
		float getQuatY(const Quaternion &v);
		float getQuatZ(const Quaternion &v);
		float getQuatW(const Quaternion &v);

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

		Vector mask(const Vector &v, const VectorMask &mask);
		Vector bitOr(const Vector &v1, const Vector &v2);

		// Quaternion
		Quaternion quatInvert(const Quaternion &q);
		Quaternion quatMultiply(const Quaternion &q1, const Quaternion &q2);
		Vector quatTransform(const Quaternion &q, const Vector &v);
		Quaternion quatAddScaled(const Quaternion &q, const Vector &vec, float scale);

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

		float clamp(float value);
	};
}

#endif /* MANTA_MATH_H */
