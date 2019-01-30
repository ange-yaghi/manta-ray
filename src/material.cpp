#include <material.h>

manta::Material::Material() {
	m_diffuseColor = math::constants::Zero;
	m_emission = math::constants::Zero;
	m_name = "";

	m_index = -1;
}

manta::Material::~Material() {

}
