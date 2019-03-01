#include <dielectric_media_interface.h>

#include <algorithm>

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

manta::math::real manta::DielectricMediaInterface::fresnelTerm(math::real cosThetaI, math::real *pdf, DIRECTION d) const {
	math::real ni, no;

	if (d == DIRECTION_IN) {
		ni = m_iorIncident;
		no = m_iorTransmitted;
	}
	else if (d == DIRECTION_OUT) {
		ni = m_iorTransmitted;
		no = m_iorIncident;
	}

	math::real cosThetaT;
	math::real sinThetaT;

	sinThetaT = ::sqrt(std::max((math::real)0.0, 1 - cosThetaI * cosThetaI));
	sinThetaT = (ni / no) * sinThetaT;

	// Total internal reflection
	if (sinThetaT >= (math::real)1.0) {
		return (math::real)1.0;
	}

	cosThetaT = ::sqrt(std::max((math::real)0.0, 1 - sinThetaT * sinThetaT));

	// Calculate pdf
	//math::real d_r_parallel = 2 * no * ni / (cosThetaT * (no * cosThetaI + ni * cosThetaT))

	math::real Rparl = ((no * cosThetaI) - (ni * cosThetaT)) /
		((no * cosThetaI) + (ni * cosThetaT));
	math::real Rperp = ((ni * cosThetaI) - (no * cosThetaT)) /
		((ni * cosThetaI) + (no * cosThetaT));

	return (Rparl * Rparl + Rperp * Rperp) / (math::real)2.0;
}

manta::math::real manta::DielectricMediaInterface::ior(DIRECTION d) {
	math::real ni, no;

	if (d == DIRECTION_IN) {
		ni = m_iorIncident;
		no = m_iorTransmitted;
	}
	else if (d == DIRECTION_OUT) {
		ni = m_iorTransmitted;
		no = m_iorIncident;
	}

	return ni / no;
}

manta::math::real manta::DielectricMediaInterface::no(DIRECTION d) {
	if (d == DIRECTION_IN) {
		return m_iorTransmitted;
	}
	else if (d == DIRECTION_OUT) {
		return m_iorIncident;
	}
}

manta::math::real manta::DielectricMediaInterface::ni(DIRECTION d) {
	if (d == DIRECTION_IN) {
		return m_iorIncident;
	}
	else if (d == DIRECTION_OUT) {
		return m_iorTransmitted;
	}
}
