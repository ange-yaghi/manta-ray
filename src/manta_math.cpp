#include <manta_math.h>

manta::math::real manta::math::get(const Vector &v, int index) {
	switch (index) {
	case 0:
		return getX(v);
	case 1:
		return getY(v);
	case 2:
		return getZ(v);
	case 3:
		return getW(v);
	default:
		return realNAN();
	}
}

manta::math::Vector manta::math::reflect(const Vector &v, const Vector &n) {
	// r = 2 * n * (v . n) - v
	math::Vector r = math::sub(
		math::mul(
			math::mul(n, math::constants::Double),
			math::dot(v, n)),
		v);
	return r;
}

manta::math::Vector manta::math::inverseReflect(const Vector &v, const Vector &r) {
	math::Vector v_dot_r = math::dot(v, r);
	math::Vector div = math::add(math::constants::Double, v_dot_r);
	div = math::add(div, v_dot_r);
	div = math::sqrt(div);

	math::Vector n = math::div(math::add(v, r), div);

	return n;
}
