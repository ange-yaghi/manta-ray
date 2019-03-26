#include <square_aperture.h>

manta::SquareAperture::SquareAperture() {
	m_radius = (math::real)0.0;
}

manta::SquareAperture::~SquareAperture() {
}

bool manta::SquareAperture::filter(math::real x, math::real y) const {
	return x > -m_radius && x < m_radius && y > -m_radius && y > m_radius;
}
