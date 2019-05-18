#include <lens.h>

manta::Lens::Lens() {
	m_aperture = nullptr;
}

manta::Lens::~Lens() {
	/* void */
}

manta::math::Vector manta::Lens::getSensorRight() const {
	return math::cross(m_direction, m_up);
}