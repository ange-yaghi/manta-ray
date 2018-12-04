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
