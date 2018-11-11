#include <manta_math_conf.h>

#if MANTA_USE_SIMD == false

#include <manta_math_single.h>

namespace math = manta::math;

math::Generic math::loadScalar(math::real s) {
	return { s, s, s, s };
}

math::Generic math::loadVector(math::real x, math::real y, math::real z, math::real w) {
	return { x, y, z, w };
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
	return q.qz;
}

math::real math::getScalar(const math::Vector &v) {
	return v.x;
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

math::Vector math::quatMultiply(const math::Quaternion &q1, const math::Quaternion &q2) {
	return constants::Zero;
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
