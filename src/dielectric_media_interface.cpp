#include <dielectric_media_interface.h>

manta::DielectricMediaInterface::DielectricMediaInterface() {
	m_iorIncident = (math::real)1.0;
	m_iorTransmitted = (math::real)1.0;
}

manta::DielectricMediaInterface::~DielectricMediaInterface() {

}

manta::math::real manta::DielectricMediaInterface::fresnelTerm(const math::Vector &i, const math::Vector &m, DIRECTION d) const {
	math::real ni, no;

	if (d == DIRECTION_IN) {
		ni = m_iorIncident;
		no = m_iorTransmitted;
	}
	else if (d == DIRECTION_OUT) {
		ni = m_iorTransmitted;
		no = m_iorIncident;
	}

	math::real c = ::abs(math::getScalar(math::dot(i, m)));
	math::real g_2 = (no * no) / (ni * ni) - (math::real)1.0 + c * c;
	if (g_2 < (math::real)0.0) {
		return (math::real)1.0;
	}
	math::real g = ::sqrt(g_2);

	// Calculate Fresnel term for dielectrics
	math::real g_min_c = g - c;
	math::real g_add_c = g + c;
	math::real t1 = (g_min_c * g_min_c) / (2 * g_add_c * g_add_c);

	math::real t2_num = c * g_add_c - 1;
	math::real t2_div = c * g_min_c + 1;

	math::real t2 = (t2_num * t2_num) / (t2_div * t2_div) + 1;

	return t1 * t2;
}
