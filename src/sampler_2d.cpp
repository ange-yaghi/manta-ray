#include <sampler_2d.h>

manta::Sampler2D::Sampler2D() {
	m_axis1 = math::constants::Zero;
	m_axis2 = math::constants::Zero;

	m_boundaryWidth = (math::real)0.0;
	m_boundaryHeight = (math::real)0.0;
}

manta::Sampler2D::~Sampler2D() {
	/* void */
}
