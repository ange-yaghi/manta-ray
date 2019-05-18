#include <starburst_approximation.h>

manta::StarburstApproximation::StarburstApproximation() {
	m_radius = (math::real)0.0;
	m_color = math::constants::One;
	m_barWidth = (math::real)0.0;
}

manta::StarburstApproximation::~StarburstApproximation() {
	/* void */
}

manta::math::real manta::StarburstApproximation::getExtents(const math::Vector &reference) const {
	if (math::getX(reference) == 0 || math::getY(reference) == 0 || math::getZ(reference) == 0) 
		return (math::real)0.0;

	return m_radius;
}

manta::math::Vector manta::StarburstApproximation::sample(math::real x, math::real y) const {
	math::real r2 = x * x + y * y;
	if (r2 < m_radius * m_radius) {
		math::Vector base = math::constants::Zero;

		if (::abs(x) < m_barWidth / 2) base = m_color;
		if (::abs(y) < m_barWidth / 2) base = m_color;

		math::real p = (math::real)1.0;
		if (r2 != 0) {
			// r^2 falloff approximation
			p = (math::real)1.0 - ((math::real)1.0 / (m_radius * m_radius)) * r2;
			p = p * p * p;
		}

		return math::mul(base, math::loadScalar(p));
	}
	else return math::constants::Zero;
}
