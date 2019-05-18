#include <circle_convolution.h>

manta::CircleConvolution::CircleConvolution() {
	m_radius = (math::real)0.0;
	m_color = math::constants::One;
}

manta::CircleConvolution::~CircleConvolution() {
	/* void */
}

manta::math::real manta::CircleConvolution::getExtents(const math::Vector &reference) const {
	return m_radius;
}

manta::math::Vector manta::CircleConvolution::sample(math::real x, math::real y) const {
	if (x * x + y * y < m_radius * m_radius) return m_color;
	else return math::constants::Zero;
}
