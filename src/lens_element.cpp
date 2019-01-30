#include <lens_element.h>

const manta::math::real manta::LensElement::IOR_CROWN_GLASS = (math::real)1.52;

manta::LensElement::LensElement() {
	m_ior = IOR_CROWN_GLASS;
}

manta::LensElement::~LensElement() {

}
