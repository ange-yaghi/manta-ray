#include <circular_aperture.h>

manta::CircularAperture::CircularAperture() {
	m_radius = (math::real)0.0;
}

manta::CircularAperture::~CircularAperture() {
	/* void */
}

bool manta::CircularAperture::filter(math::real x, math::real y) const {
	return (x * x + y * y) < (m_radius * m_radius);
}
