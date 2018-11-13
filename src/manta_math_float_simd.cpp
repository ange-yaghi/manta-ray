#include <manta_math_conf.h>

#if MANTA_USE_SIMD
#if MANTA_PRECISION == MANTA_PRECISION_FLOAT

#include <manta_math.h>

#include <math.h>
#include <random>

namespace math = manta::math;

math::Vector manta::math::uniformRandom4(real range) {
	real r = (rand() % RAND_MAX) / ((real)(RAND_MAX - 1));
	return loadScalar(range * r);
}

float manta::math::uniformRandom(real range) {
	real r = (rand() % RAND_MAX) / ((real)(RAND_MAX - 1));
	return range * r;
}

math::Generic math::loadScalar(real s) {
	return _mm_set_ps(s, s, s, s);
}

math::Generic math::loadVector(real x, real y, real z, real w) {
	return _mm_set_ps(w, z, y, x);
}

math::Generic math::loadVector(const Vector4 &v) {
	return _mm_set_ps(v.w, v.z, v.y, v.x);
}

math::Generic math::loadVector(const Vector3 &v, real w) {
	return _mm_set_ps(w, v.z, v.y, v.x);
}

math::Generic manta::math::loadVector(const Vector2 &v1) {
	return _mm_set_ps(0.0, 0.0, v1.y, v1.x);
}

math::Generic math::loadVector(const Vector2 &v1, const Vector2 &v2) {
	return _mm_set_ps(v2.y, v2.x, v1.y, v1.x);
}

math::Quaternion math::loadQuaternion(real angle, const Vector &axis) {
	real sinAngle = (real)sin(angle / (real)2.0);
	real cosAngle = (real)cos(angle / (real)2.0);

	Vector newAxis = _mm_shuffle_ps(axis, axis, _MM_SHUFFLE(2, 1, 0, 3));
	newAxis = _mm_and_ps(newAxis, constants::MaskOffX);
	newAxis = _mm_mul_ps(newAxis, math::loadScalar(sinAngle));
	newAxis = _mm_or_ps(newAxis, math::loadVector(cosAngle));

	newAxis = manta::math::normalize(newAxis);

	return newAxis;
}

math::Vector4 math::getVector4(const Vector &v) {
	Vector4 r;
	r.x = v.m128_f32[0];
	r.y = v.m128_f32[1];
	r.z = v.m128_f32[2];
	r.w = v.m128_f32[3];

	return r;
}

math::Vector3 math::getVector3(const Vector &v) {
	Vector3 r;
	r.x = v.m128_f32[0];
	r.y = v.m128_f32[1];
	r.z = v.m128_f32[2];

	return r;
}

math::Vector2 manta::math::getVector2(const Vector &v) {
	math::Vector2 r;
	r.x = v.m128_f32[0];
	r.y = v.m128_f32[1];

	return r;
}

float math::getScalar(const Vector &v) {
	return v.m128_f32[0];
}

float math::getX(const Vector &v) {
	return v.m128_f32[0];
}

float math::getY(const Vector &v) {
	return v.m128_f32[1];
}

float manta::math::getZ(const Vector &v) {
	return v.m128_f32[2];
}

float manta::math::getW(const Vector &v) {
	return v.m128_f32[3];
}

float manta::math::getQuatX(const Quaternion &v) {
	return v.m128_f32[1];
}

float manta::math::getQuatY(const Quaternion &v) {
	return v.m128_f32[2];
}

float manta::math::getQuatZ(const Quaternion &v) {
	return v.m128_f32[3];
}

float manta::math::getQuatW(const Quaternion &v) {
	return v.m128_f32[0];
}

math::Generic manta::math::add(const Generic &v1, const Generic &v2) {
	return _mm_add_ps(v1, v2);
}

math::Generic manta::math::sub(const Generic &v1, const Generic &v2) {
	return _mm_sub_ps(v1, v2);
}

math::Generic manta::math::div(const Generic &v1, const Generic &v2) {
	return _mm_div_ps(v1, v2);
}

math::Generic manta::math::mul(const Generic &v1, const Generic &v2) {
	return _mm_mul_ps(v1, v2);
}

math::Vector math::dot(const Vector &v1, const Vector &v2) {
	Vector t0 = _mm_mul_ps(v1, v2);
	Vector t1 = _mm_shuffle_ps(t0, t0, _MM_SHUFFLE(1,0,3,2));
	Vector t2 = _mm_add_ps(t0, t1);
	Vector t3 = _mm_shuffle_ps(t2, t2, _MM_SHUFFLE(2,3,0,1));
	Vector dot = _mm_add_ps(t3, t2);

	return (dot);
}

math::Vector math::dot3(const Vector &v1, const Vector &v2) {
	Vector t0 = _mm_mul_ps(v1, v2);
	t0 = _mm_and_ps(t0, constants::MaskOffW);

	Vector t1 = _mm_shuffle_ps(t0, t0, _MM_SHUFFLE(1,0,3,2));
	Vector t2 = _mm_add_ps(t0, t1);
	Vector t3 = _mm_shuffle_ps(t2, t2, _MM_SHUFFLE(2,3,0,1));
	Vector dot = _mm_add_ps(t3, t2);

	return (dot);
}

math::Vector math::cross(const Vector &v1, const Vector &v2) {

	// STOLEN FROM XNA MATH

	// y1, z1, x1, w1
    Vector t1 = _mm_shuffle_ps(v1, v1, _MM_SHUFFLE(3,0,2,1));

	// z2, x2, y2, w2
    Vector t2 = _mm_shuffle_ps(v2, v2, _MM_SHUFFLE(3,1,0,2));

    Vector vResult = _mm_mul_ps(t1, t2);

    // z1, x1, y1, w1
    t1 = _mm_shuffle_ps(t1, t1, _MM_SHUFFLE(3,0,2,1));

    // y2, z2, x2, w2
    t2 = _mm_shuffle_ps(t2, t2, _MM_SHUFFLE(3,1,0,2));

    // Perform the right operation
    t1 = _mm_mul_ps(t1, t2);

    // Subract the right from left, and return answer
    vResult = _mm_sub_ps(vResult, t1);

    // Set w to zero
    return _mm_and_ps(vResult, constants::MaskOffW);
}

math::Vector math::sqrt(const Vector & v) {
	return _mm_sqrt_ps(v);
}

math::Vector math::magnitudeSquared3(const Vector &v) {
	return math::dot3(v, v);
}

math::Vector math::magnitude(const Vector &v) {
	Vector selfDot = math::dot(v, v);
	return _mm_sqrt_ps(selfDot);
}

math::Vector math::normalize(const Vector &v) {
	return math::div(v, math::magnitude(v));
}

math::Vector math::negate(const Vector &v) {
	return manta::math::mul(v, constants::Negate);
}

math::Vector manta::math::negate3(const Vector &v) {
	return manta::math::mul(v, constants::Negate3);
}

math::Vector math::mask(const Vector &v, const VectorMask &mask) {
	return _mm_and_ps(v, mask.vector);
}

math::Vector manta::math::bitOr(const Vector &v1, const Vector &v2) {
	return _mm_or_ps(v1, v2);
}

// Quaternion

math::Quaternion math::quatInvert(const Quaternion &q) {
	Quaternion nq = math::loadVector((real)1.0, (real)-1.0, (real)-1.0, (real)-1.0);

	return math::mul(nq, q);
}

math::Quaternion manta::math::quatMultiply(const Quaternion &q1, const Quaternion &q2) {
	Generic w1 = _mm_replicate_x_ps(q1);
	Generic x1 = _mm_replicate_y_ps(q1);
	Generic y1 = _mm_replicate_z_ps(q1);
	Generic z1 = _mm_replicate_w_ps(q1);

	Generic m1 = q2;
	Generic m2 = _mm_shuffle_ps(q2, q2, _MM_SHUFFLE(2, 3, 0, 1)); // xwzy
	Generic m3 = _mm_shuffle_ps(q2, q2, _MM_SHUFFLE(1, 0, 3, 2)); // yzwx
	Generic m4 = _mm_shuffle_ps(q2, q2, _MM_SHUFFLE(0, 1, 2, 3)); // zyxw

	Generic sgn2 = _mm_set_ps(1, -1, 1, -1);
	Generic sgn3 = _mm_set_ps(-1, 1, 1, -1);
	Generic sgn4 = _mm_set_ps(1, 1, -1, -1);

	Generic prod1 = _mm_mul_ps(w1, m1); // q1.x*q2
	Generic prod2 = _mm_mul_ps(_mm_mul_ps(x1, sgn2), m2);
	Generic prod3 = _mm_mul_ps(_mm_mul_ps(y1, sgn3), m3);
	Generic prod4 = _mm_mul_ps(_mm_mul_ps(z1, sgn4), m4);

	Generic result = _mm_add_ps( _mm_add_ps(prod1, prod2), _mm_add_ps(prod3, prod4) );

	return (Quaternion)result;
}

math::Vector math::quatTransform(const Quaternion &q, const Vector &v) {
	Vector transformed = _mm_shuffle_ps(v, v, _MM_SHUFFLE(2, 1, 0, 3)); // wxyz
	Quaternion inv = math::quatInvert(q);

	transformed = math::quatMultiply(q, transformed);
	transformed = math::quatMultiply(transformed, inv);

	return _mm_shuffle_ps(transformed, transformed, _MM_SHUFFLE(0, 3, 2, 1)); // wxyz;
}

math::Quaternion math::quatAddScaled(const Quaternion &q, const Vector &vec, real scale) {
	Generic n = _mm_shuffle_ps(vec, vec, _MM_SHUFFLE(2, 1, 0, 3));
	n = _mm_and_ps(n, constants::MaskOffX);

	n = _mm_mul_ps(n, manta::math::loadScalar(scale));

	Quaternion q2 = n;
	Quaternion m1 = manta::math::quatMultiply(q2, q);

	Quaternion ret = _mm_add_ps(q, _mm_mul_ps(m1, constants::Half));

	return manta::math::normalize(ret);
}

// Matrices

math::Matrix math::loadIdentity() {
	Matrix r;

	r.rows[0] = constants::IdentityRow1;
	r.rows[1] = constants::IdentityRow2;
	r.rows[2] = constants::IdentityRow3;
	r.rows[3] = constants::IdentityRow4;

	return r;
}

math::Matrix manta::math::loadMatrix(const Vector &r1, const Vector &r2, const Vector &r3, const Vector &r4) {
	Matrix r;
	r.rows[0] = r1;
	r.rows[1] = r2;
	r.rows[2] = r3;
	r.rows[3] = r4;

	return r;
}

math::Matrix math::loadMatrix(const Quaternion &quat) {
	// 21 instruction implementation

	Generic q = quat;
	Generic nq = _mm_sub_ps(math::constants::Zero, q);
	Generic qq = _mm_add_ps(q, q);
	Generic q2 = _mm_mul_ps(qq, q);

	Generic xyxx = _mm_shuffle_ps(q, q, _MM_SHUFFLE(1, 1, 2, 1));
	Generic yzzy = _mm_shuffle_ps(qq, qq, _MM_SHUFFLE(2, 3, 3, 2));
	Generic zxyz = _mm_shuffle_ps(qq, qq, _MM_SHUFFLE(3, 2, 1, 3));
	Generic wwww = _mm_shuffle_ps(q, nq, _MM_SHUFFLE(0, 0, 0, 0));

	Generic i1 = _mm_mul_ps(xyxx, yzzy); // [2xy, 2yz, 2xz, 2xy]
	Generic i2 =  _mm_mul_ps(zxyz, wwww); // [2zw, 2xw, -2yw, -2zw ]
	Generic calc1 = _mm_add_ps(i1, i2);

	// Stage 2

	Generic y2_x2_x2_w2 = _mm_shuffle_ps(q2, q2, _MM_SHUFFLE(0, 1, 1, 2));
	Generic z2_z2_y2_w2 = _mm_shuffle_ps(q2, q2, _MM_SHUFFLE(0, 2, 3, 3));

	Generic calc2 = _mm_sub_ps(math::constants::One, _mm_add_ps(y2_x2_x2_w2, z2_z2_y2_w2));
	calc2 = _mm_and_ps(calc2, math::constants::MaskOffW.vector);

	// Stage 3
	
	Generic calc3 = _mm_sub_ps(i1, i2);

	// Assembly

	// 2xy - 2zw -> 3
	// 2xz - 2yw -> 2
	// 2xy + 2zw -> 0
	// 2yz + 2xw -> 1

	Generic asm1 = _mm_shuffle_ps(calc2, calc1, _MM_SHUFFLE(2, 0, 3, 0));
	asm1 = _mm_shuffle_ps(asm1, asm1, _MM_SHUFFLE(1, 3, 2, 0));

	Generic asm2 = _mm_shuffle_ps(calc2, calc1, _MM_SHUFFLE(1, 3, 3, 1));
	asm2 = _mm_shuffle_ps(asm2, asm2, _MM_SHUFFLE(1, 3, 0, 2));

	Generic asm3 = _mm_shuffle_ps(calc3, calc2, _MM_SHUFFLE(3, 2, 1, 2));
	// No need to shuffle this one

	Matrix ret = manta::math::loadMatrix(asm1, asm2, asm3, math::constants::IdentityRow4);

	return ret;
}

math::Matrix math::loadMatrix(const Quaternion &quat, const Vector &origin) {
	Generic q = quat;
	Generic nq = _mm_sub_ps(constants::Zero, q);
	Generic qq = _mm_add_ps(q, q);
	Generic q2 = _mm_mul_ps(qq, q);

	Generic xxxy = _mm_shuffle_ps(q, q, _MM_SHUFFLE(2, 1, 1, 1));
	Generic zyyz = _mm_shuffle_ps(qq, qq, _MM_SHUFFLE(3, 2, 2, 3));
	Generic yzzx = _mm_shuffle_ps(qq, qq, _MM_SHUFFLE(1, 3, 3, 2));
	Generic wwww = _mm_shuffle_ps(q, nq, _MM_SHUFFLE(0, 0, 0, 0));

	Generic i1 = _mm_mul_ps(xxxy, zyyz);	// [2xz, 2xy, 2xy, 2yz]
	Generic i2 =  _mm_mul_ps(yzzx, wwww); // [2yw, 2zw, -2zw, -2xw]
	Generic calc1 = _mm_add_ps(i1, i2);	// [2xz - 2yw, 2xy + 2zy, 2xy - 2zy, 2yz - 2xw]

	// Stage 2

	Generic y2_x2_x2_w2 = _mm_shuffle_ps(q2, q2, _MM_SHUFFLE(0, 1, 1, 2));
	Generic z2_z2_y2_w2 = _mm_shuffle_ps(q2, q2, _MM_SHUFFLE(0, 2, 3, 3));

	Generic calc2 = _mm_sub_ps(constants::One, _mm_add_ps(y2_x2_x2_w2, z2_z2_y2_w2));
	calc2 = _mm_and_ps(calc2, constants::MaskOffW.vector);

	// Stage 3
	
	Generic calc3 = _mm_sub_ps(i1, i2);	// [2xz + 2yw, 2xy - 2zy, 2xy + 2zy, 2yz + 2xw]

	// Assembly

	// 2xz + 2yw -> 0
	// 2xy + 2zw -> 1
	// 2xy - 2zw -> 2
	// 2yz - 2xw -> 3

	Generic asm1 = _mm_shuffle_ps(calc2, calc1, _MM_SHUFFLE(0, 2, 3, 0));
	asm1 = _mm_shuffle_ps(asm1, asm1, _MM_SHUFFLE(1, 3, 2, 0));

	Generic asm2 = _mm_shuffle_ps(calc2, calc1, _MM_SHUFFLE(3, 1, 3, 1));
	asm2 = _mm_shuffle_ps(asm2, asm2, _MM_SHUFFLE(1, 3, 0, 2));

	Generic asm3 = _mm_shuffle_ps(calc3, calc2, _MM_SHUFFLE(3, 2, 3, 0));
	// No need to shuffle this one

	Generic asm4 = _mm_and_ps(origin, constants::MaskOffW.vector);
	asm4 = _mm_add_ps(asm4, constants::IdentityRow4);

	Matrix ret = math::transpose(math::loadMatrix(asm1, asm2, asm3, asm4));

	return ret;
}

void math::loadMatrix(const Quaternion &quat, const Vector &origin, Matrix *full, Matrix *orientation) {
	Generic q = quat;
	Generic nq = _mm_sub_ps(constants::Zero, q);
	Generic qq = _mm_add_ps(q, q);
	Generic q2 = _mm_mul_ps(qq, q);

	Generic xxxy = _mm_shuffle_ps(q, q, _MM_SHUFFLE(2, 1, 1, 1));
	Generic zyyz = _mm_shuffle_ps(qq, qq, _MM_SHUFFLE(3, 2, 2, 3));
	Generic yzzx = _mm_shuffle_ps(qq, qq, _MM_SHUFFLE(1, 3, 3, 2));
	Generic wwww = _mm_shuffle_ps(q, nq, _MM_SHUFFLE(0, 0, 0, 0));

	Generic i1 = _mm_mul_ps(xxxy, zyyz); // [2xz, 2xy, 2xy, 2yz]
	Generic i2 =  _mm_mul_ps(yzzx, wwww); // [2yw, 2zw, -2zw, -2xw]
	Generic calc1 = _mm_add_ps(i1, i2);

	// Stage 2

	Generic y2_x2_x2_w2 = _mm_shuffle_ps(q2, q2, _MM_SHUFFLE(0, 1, 1, 2));
	Generic z2_z2_y2_w2 = _mm_shuffle_ps(q2, q2, _MM_SHUFFLE(0, 2, 3, 3));

	Generic calc2 = _mm_sub_ps(constants::One, _mm_add_ps(y2_x2_x2_w2, z2_z2_y2_w2));
	calc2 = _mm_and_ps(calc2, constants::MaskOffW.vector);

	// Stage 3
	
	Generic calc3 = _mm_sub_ps(i1, i2);

	// Assembly

	// 2xz + 2yw -> 0
	// 2xy + 2zw -> 1
	// 2xy - 2zw -> 2
	// 2yz - 2xw -> 3

	Generic asm1 = _mm_shuffle_ps(calc2, calc1, _MM_SHUFFLE(0, 2, 3, 0));
	asm1 = _mm_shuffle_ps(asm1, asm1, _MM_SHUFFLE(1, 3, 2, 0));

	Generic asm2 = _mm_shuffle_ps(calc2, calc1, _MM_SHUFFLE(3, 1, 3, 1));
	asm2 = _mm_shuffle_ps(asm2, asm2, _MM_SHUFFLE(1, 3, 0, 2));

	Generic asm3 = _mm_shuffle_ps(calc3, calc2, _MM_SHUFFLE(3, 2, 3, 0));
	// No need to shuffle this one

	*orientation = math::transpose(math::loadMatrix(asm1, asm2, asm3, constants::IdentityRow4));

	Generic asm4 = _mm_and_ps(origin, constants::MaskOffW.vector);
	asm4 = _mm_add_ps(asm4, constants::IdentityRow4);

	*full = math::transpose(math::loadMatrix(asm1, asm2, asm3, asm4));
}

math::Matrix manta::math::transpose(const Matrix &m) {
	Matrix r = m;
	_MM_TRANSPOSE4_PS(r.rows[0], r.rows[1], r.rows[2], r.rows[3]);

	return r;
}

math::Matrix manta::math::orthogonalInverse(const Matrix &m) {
	Matrix r = m;

	_MM_TRANSPOSE4_PS(r.rows[0], r.rows[1], r.rows[2], r.rows[3]);

	// RTv
	// (Tinv)(Rinv)v

	Matrix r_inv = loadMatrix(r.rows[0], r.rows[1], r.rows[2], manta::math::constants::IdentityRow4);
	Matrix t_inv = translationTransform(manta::math::negate3(r.rows[3]));

	r = matMult(r_inv, t_inv);

	return r;
}

math::Matrix44 math::getMatrix44(const Matrix &m) {
	Matrix44 r;
	r.rows[0] = manta::math::getVector4(m.rows[0]);
	r.rows[1] = manta::math::getVector4(m.rows[1]);
	r.rows[2] = manta::math::getVector4(m.rows[2]);
	r.rows[3] = manta::math::getVector4(m.rows[3]);

	return r;
}

math::Matrix33 math::getMatrix33(const Matrix &m) {
	Matrix33 r;
	r.rows[0] = manta::math::getVector3(m.rows[0]);
	r.rows[1] = manta::math::getVector3(m.rows[1]);
	r.rows[2] = manta::math::getVector3(m.rows[2]);

	return r;
}

math::Vector math::extendVector(const Vector &v) {
	Vector ret = v;
	ret = mask(v, constants::MaskOffW);
	ret = _mm_or_ps(ret, constants::IdentityRow4);

	return ret;
}

math::Vector math::matMult(const Matrix &m, const Vector &v) {
	Matrix t = m;
	_MM_TRANSPOSE4_PS(t.rows[0], t.rows[1], t.rows[2], t.rows[3]);

	//ysVector exV = ExtendVector(v);

	Vector r;
	r = _mm_mul_ps(_mm_replicate_x_ps(v), t.rows[0]);
	r = _mm_madd_ps(_mm_replicate_y_ps(v), t.rows[1], r);
	r = _mm_madd_ps(_mm_replicate_z_ps(v), t.rows[2], r);
	r = _mm_madd_ps(_mm_replicate_w_ps(v), t.rows[3], r);

	return r;
}

math::Matrix math::matMult(const Matrix &m1, const Matrix &m2) {
	Matrix r;

	for(int i=0; i < 4; i++) {
		r.rows[i] = _mm_mul_ps(_mm_replicate_x_ps(m1.rows[i]), m2.rows[0]);
		r.rows[i] = _mm_madd_ps(_mm_replicate_y_ps(m1.rows[i]), m2.rows[1], r.rows[i]);
		r.rows[i] = _mm_madd_ps(_mm_replicate_z_ps(m1.rows[i]), m2.rows[2], r.rows[i]);
		r.rows[i] = _mm_madd_ps(_mm_replicate_w_ps(m1.rows[i]), m2.rows[3], r.rows[i]);
	}

	return r;
}

math::Matrix math::frustrumPerspective(float fovy, float aspect, float near, float far) {
	float sinfov = (float)sin(fovy / 2.0);
	float cosfov = (float)cos(fovy / 2.0);

	float height = cosfov / sinfov;
	float width = height / aspect;
	
	Vector row1 = loadVector(width,	0,			0,								0);
	Vector row2 = loadVector(0,		height,		0,								0);
	Vector row3 = loadVector(0,		0,			(far)/(far - near),				1.0);
	Vector row4 = loadVector(0,		0,			-(far*near)/(far - near),		0);

	return loadMatrix(row1, row2, row3, row4);
}

math::Matrix math::orthographicProjection(float width, float height, float near, float far) {
	float fRange = 1.0f / (far - near);

	Vector row1 = math::loadVector(2.0f / width,	0.0f,				0.0f,					0.0f);
	Vector row2 = math::loadVector(0.0f,			2.0f / height,		0.0f,					0.0f);
	Vector row3 = math::loadVector(0.0f,			0.0f,				2 * fRange,			0.0f);
	Vector row4 = math::loadVector(0.0f,			0.0f,				-fRange * near,			1.0f);

	return loadMatrix(row1, row2, row3, row4);
}

math::Matrix math::cameraTarget(const Vector &eye, const Vector &target, const Vector &up) {
	Vector R2 = sub(target, eye);
	R2 = normalize(R2);

	Vector R0 = cross(R2, up);
	R0 = normalize(R0);

	Vector R1 = cross(R0, R2);

	Vector negEyePos = negate(eye);

	Vector D0 = manta::math::dot(R0, negEyePos);
	Vector D1 = manta::math::dot(R1, negEyePos);
	Vector D2 = manta::math::dot(R2, negEyePos);

	R0 = mask(R0, constants::MaskOffW);
	R1 = mask(R1, constants::MaskOffW);
	R2 = mask(R2, constants::MaskOffW);

	D0 = mask(D0, constants::MaskKeepW);
	D1 = mask(D1, constants::MaskKeepW);
	D2 = mask(D2, constants::MaskKeepW);

	D0 = bitOr(R0, D0);
	D1 = bitOr(R1, D1);
	D2 = bitOr(R2, D2);

	Matrix r;

	r.rows[0] = D0;
	r.rows[1] = D1;
	r.rows[2] = D2;
	r.rows[3] =constants::IdentityRow4;

	r = transpose(r);
	
	return r;
}

math::Matrix math::translationTransform(const Vector &translation) {
	Matrix r;

	r.rows[0] = constants::IdentityRow1;
	r.rows[1] = constants::IdentityRow2;
	r.rows[2] = constants::IdentityRow3;

	Vector trans = mask(translation, constants::MaskOffW);
	r.rows[3] = bitOr(trans, constants::IdentityRow4);

	r = transpose(r);

	return r;
}

math::Matrix math::scaleTransform(const Vector &scale) {
	Matrix r;

	r.rows[0] = mask(scale, constants::MaskKeepX);
	r.rows[1] = mask(scale, constants::MaskKeepY);
	r.rows[2] = mask(scale, constants::MaskKeepZ);
	r.rows[3] = constants::IdentityRow4;

	return r;
}

math::Matrix math::rotationTransform(const Vector &axis, float angle) {
	// TEMP
	// BAD IMPLEMENTATION

	Matrix r;

	Vector naxis = normalize(axis);

	float ux = getX(naxis);
	float uy = getY(naxis);
	float uz = getZ(naxis);

	float cos_a = (float)cos(angle);
	float sin_a = (float)sin(angle);

	r.rows[0] = loadVector(cos_a + ux*ux * (1 - cos_a), ux*uy*(1-cos_a) - uz*sin_a, ux*uz*(1-cos_a) + uy*sin_a);
	r.rows[1] = loadVector(uy*ux*(1-cos_a)+uz*sin_a, cos_a+uy*uy*(1-cos_a), uy*uz*(1-cos_a)-ux*sin_a);
	r.rows[2] = loadVector(uz*ux*(1-cos_a)-uy*sin_a, uz*uy*(1-cos_a)+ux*sin_a, cos_a+uz*uz*(1-cos_a));
	r.rows[3] = constants::IdentityRow4;

	return r;
}

math::Vector math::getTranslationPart(const Matrix &mat) {
	Matrix r = transpose(mat);

	return r.rows[3];
}

float math::clamp(float value) {
	if (value > 1.0f) {
		return 1.0f;
	}
	else if (value < 0.0f) {
		return 0.0f;
	}
	else {
		return value;
	}
}

#endif /* MANTA_USE_SIMD */
#endif /* MANTA_PRECISION */
