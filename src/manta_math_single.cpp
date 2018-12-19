#include <manta_math_conf.h>

#if MANTA_USE_SIMD == false

#include <manta_math.h>

namespace math = manta::math;

math::Generic math::loadScalar(math::real s) {
	return { s, s, s, s };
}

math::Generic math::loadVector(math::real x, math::real y, math::real z, math::real w) {
	return { x, y, z, w };
}

math::Generic math::loadVector(const Vector4 &v) {
	return { v.x, v.y, v.z, v.w };
}

math::Generic math::loadVector(const Vector3 &v, real w) {
	return { v.x, v.y, v.z, w };
}

math::Generic math::loadVector(const Vector2 &v) {
	return { v.x, v.y, (real)0.0, (real)0.0 };
}

math::Generic math::loadVector(const Vector2 &v1, const Vector2 &v2) {
	return { v1.x, v1.y, v2.x, v2.y };
}

math::Quaternion math::loadQuaternion(real angle, const Vector &axis) {
	real sinAngle = (real)sin(angle / (real)2.0);
	real cosAngle = (real)cos(angle / (real)2.0);

	Quaternion q = { (real)0.0, axis.x, axis.y, axis.z };
	q.qx *= sinAngle;
	q.qy *= sinAngle;
	q.qz *= sinAngle;
	q.qw = cosAngle;

	q = manta::math::normalize(q);

	return q;
}

math::Generic math::componentMax(const Generic &a, const Generic &b) {
	math::Generic result = a;
	if (b.x > a.x) result.x = b.x;
	if (b.y > a.y) result.y = b.y;
	if (b.z > a.z) result.z = b.z;
	if (b.w > a.w) result.w = b.w;
	return result;
}

math::Generic math::componentMin(const Generic &a, const Generic &b) {
	math::Generic result = a;
	if (b.x < a.x) result.x = b.x;
	if (b.y < a.y) result.y = b.y;
	if (b.z < a.z) result.z = b.z;
	if (b.w < a.w) result.w = b.w;
	return result;
}

math::real math::getX(const math::Vector &v) {
	return v.x;
}

math::real math::getY(const math::Vector &v) {
	return v.y;
}

math::real math::getZ(const math::Vector &v) {
	return v.z;
}

math::real math::getW(const math::Vector &v) {
	return v.w;
}

math::real math::getQuatX(const math::Quaternion &q) {
	return q.qx;
}

math::real math::getQuatY(const math::Quaternion &q) {
	return q.qy;
}

math::real math::getQuatZ(const math::Quaternion &q) {
	return q.qz;
}

math::real math::getQuatW(const math::Quaternion &q) {
	return q.qw;
}

math::real math::getScalar(const math::Vector &v) {
	return v.x;
}

math::Vector4 math::getVector4(const math::Vector &v) {
	return Vector4((float)v.x, (float)v.y, (float)v.z, (float)v.w);
}

math::Vector3 math::getVector3(const math::Vector &v) {
	return Vector3((float)v.x, (float)v.y, (float)v.z);
}

math::Vector2 math::getVector2(const math::Vector &v) {
	return Vector2((float)v.x, (float)v.y);
}

math::Generic math::add(const math::Generic &v1, const math::Generic &v2) {
	return { v1.x + v2.x, v1.y + v2.y, v1.z + v2.z, v1.w + v2.w };
}

math::Generic math::sub(const math::Generic &v1, const math::Generic &v2) {
	return { v1.x - v2.x, v1.y - v2.y, v1.z - v2.z, v1.w - v2.w };
}

math::Generic math::mul(const math::Generic &v1, const math::Generic &v2) {
	return { v1.x * v2.x, v1.y * v2.y, v1.z * v2.z, v1.w * v2.w };
}

math::Generic math::div(const math::Generic &v1, const math::Generic &v2) {
	return { v1.x / v2.x, v1.y / v2.y, v1.z / v2.z, v1.w / v2.w };
}

math::Vector math::dot(const math::Vector &v1, const math::Vector &v2) {
	return loadScalar(v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w);
}

math::Vector math::dot3(const math::Vector &v1, const math::Vector &v2) {
	return loadScalar(v1.x * v2.x + v1.y * v2.y + v1.z * v2.z);
}

math::Vector math::cross(const math::Vector &v1, const math::Vector &v2) {
	return {
		v1.y * v2.z - v1.z * v2.y,
		v1.z * v2.x - v1.x * v2.z,
		v1.x * v2.y - v1.y * v2.x
	};
}

math::Vector math::sqrt(const math::Vector &v) {
	return {
		(real)::sqrt(v.x),
		(real)::sqrt(v.y),
		(real)::sqrt(v.z),
		(real)::sqrt(v.w)
	};
}

math::Vector math::magnitudeSquared3(const math::Vector &v) {
	return dot(v, v);
}

math::Vector math::magnitude(const math::Vector &v) {
	return sqrt(magnitudeSquared3(v));
}

math::Vector math::normalize(const math::Vector &v) {
	return div(v, magnitude(v));
}

math::Vector math::negate(const math::Vector &v) {
	return { -v.x, -v.y, -v.z, -v.w };
}

math::Vector math::negate3(const math::Vector &v) {
	return { -v.x, -v.y, -v.z, v.w };
}

math::Vector math::quatInvert(const math::Quaternion &q) {
	return { q.qw, -q.qx, -q.qy, -q.qz };
}

math::Quaternion math::quatMultiply(const math::Quaternion &a, const math::Quaternion &b) {
	return {
		a.qw * b.qw - a.qx * b.qx - a.qy * b.qy - a.qz * b.qz,
		a.qw * b.qx + a.qx * b.qw + a.qy * b.qz - a.qz * b.qy,
		a.qw * b.qy - a.qx * b.qz + a.qy * b.qw + a.qz * b.qx,
		a.qw * b.qz + a.qx * b.qy - a.qy * b.qx + a.qz * b.qw
	};
}

math::Vector math::quatTransform(const Quaternion &q, const Vector &vec) {
	Quaternion transformed = { vec.w, vec.x, vec.y, vec.z };
	Quaternion inv = math::quatInvert(q);

	transformed = math::quatMultiply(q, transformed);
	transformed = math::quatMultiply(transformed, inv);

	return { transformed.qx, transformed.qy, transformed.qz, transformed.qw };
}

math::Vector math::uniformRandom4(math::real range) {
	real r = (rand() % RAND_MAX) / ((real)(RAND_MAX - 1));
	return loadScalar(r * range);
}

math::real math::uniformRandom(math::real range) {
	real r = (rand() % RAND_MAX) / ((real)(RAND_MAX - 1));
	return r * range;
}

math::real math::clamp(math::real value) {
	if (value < (real)0.0) {
		return (real)0.0;
	}
	else if (value > (real)1.0) {
		return (real)1.0;
	}
	else {
		return value;
	}
}

#endif /* MANTA_USE_SIMD */
