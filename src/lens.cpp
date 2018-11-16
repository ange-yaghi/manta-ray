#include <lens.h>

manta::Lens::Lens() {
	r_aperture = nullptr;
}

manta::Lens::~Lens() {

}

manta::math::Vector manta::Lens::getSensorUp() const {
	return m_up;
}

manta::math::Vector manta::Lens::getSensorRight() const {
	return math::cross(m_direction, m_up);
}