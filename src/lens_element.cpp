#include <lens_element.h>

const manta::math::real manta::LensElement::IOR_CROWN_GLASS = (math::real)1.52;

manta::LensElement::LensElement() {
	m_position = math::constants::Zero;
	m_direction = math::constants::Zero;
	m_depth = (math::real)0.0;
	m_radius = (math::real)0.0;

	m_ior = IOR_CROWN_GLASS;
}

manta::LensElement::~LensElement() {
	/* void */
}
