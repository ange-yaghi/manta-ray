#include <microfacet_glass_bsdf.h>

#include <intersection_point.h>
#include <microfacet_distribution.h>

manta::MicrofacetGlassBSDF::MicrofacetGlassBSDF() {

}

manta::MicrofacetGlassBSDF::~MicrofacetGlassBSDF() {

}

void manta::MicrofacetGlassBSDF::initializeSessionMemory(const IntersectionPoint *surfaceInteraction, NodeSessionMemory *memory, StackAllocator *stackAllocator) const {

}

manta::math::Vector manta::MicrofacetGlassBSDF::sampleF(const IntersectionPoint *surfaceInteraction, const math::Vector &i, math::Vector *o, math::real *pdf, StackAllocator *stackAllocator) const {
	constexpr math::Vector reflect = { (math::real)-1.0, (math::real)-1.0, (math::real)1.0, (math::real)1.0 };
	
	math::real u = math::uniformRandom();

	// Allocate required memory
	NodeSessionMemory s;
	m_distribution->initializeSessionMemory(surfaceInteraction, &s, stackAllocator);

	// Generate microfacet normal
	math::Vector m = m_distribution->generateMicrosurfaceNormal(&s);

	math::real ior = m_mediaInterface->ior(surfaceInteraction->m_direction);
	math::real F = m_mediaInterface->fresnelTerm(i, m, surfaceInteraction->m_direction);
	math::real o_dot_m;

	math::Vector f;

	math::real rPdf = 0.0, tPdf = 0.0;

	if (u > (math::real)0.5) {
		// Reflection
		math::Vector ri = math::reflect(i, m);
		o_dot_m = math::getScalar(math::dot(ri, m));

		*o = ri;

		math::real cosThetaO = ::abs(math::getZ(*o));
		math::real cosThetaI = ::abs(math::getZ(i));

		if (o_dot_m <= (math::real)0.0 ||
			cosThetaO <= (math::real)0.0 ||
			cosThetaI <= (math::real)0.0) {
			// Free all memory
			m_distribution->destroySessionMemory(&s, stackAllocator);

			*pdf = 0.0;
			return math::constants::Zero;
		}

		rPdf = (m_distribution->calculatePDF(m, &s) / ::abs(4 * o_dot_m));

		// Calculate reflectivity
		math::Vector reflectivity = math::loadScalar(
			m_distribution->calculateDistribution(m, &s) * m_distribution->bidirectionalShadowMasking(i, *o, m, &s) * F / (4 * cosThetaI * cosThetaO));

		// Free all memory
		m_distribution->destroySessionMemory(&s, stackAllocator);

		f = reflectivity;
	}
	else {
		// Refraction
		math::Vector rt;
		if (!refract(i, m, ior, &rt)) {
			// Free all memory
			m_distribution->destroySessionMemory(&s, stackAllocator);

			*pdf = (math::real)0.0;
			return math::constants::Zero;
		}

		if ((math::getZ(rt) > 0) == (math::getZ(i) > 0)) {
			// Free all memory
			m_distribution->destroySessionMemory(&s, stackAllocator);

			*pdf = (math::real)0.0;
			return math::constants::Zero;
		}

		o_dot_m = math::getScalar(math::dot(rt, m));
		math::real i_dot_m = math::getScalar(math::dot(i, m));

		*o = rt;

		math::real cosThetaI = ::abs(math::getZ(i));
		math::real cosThetaO = ::abs(math::getZ(rt));

		if (i_dot_m <= (math::real)0.0 ||
			o_dot_m == (math::real)0.0 ||
			cosThetaO == (math::real)0.0 ||
			cosThetaI == (math::real)0.0) {
			// Free all memory
			m_distribution->destroySessionMemory(&s, stackAllocator);

			*pdf = (math::real)0.0;
			return math::constants::Zero;
		}

		math::real no = m_mediaInterface->no(surfaceInteraction->m_direction);
		math::real ni = m_mediaInterface->ni(surfaceInteraction->m_direction);
		math::real jacobian = no * no * ::abs(o_dot_m);
		math::real jacobian_div = ni * ::abs(i_dot_m) + no * ::abs(o_dot_m);
		jacobian /= (jacobian_div * jacobian_div);

		tPdf = m_distribution->calculatePDF(m, &s) * jacobian;

		// Calculate transmitance
		math::real Ft_num = (1 / (ior * ior)) * m_distribution->calculateDistribution(m, &s) * m_distribution->bidirectionalShadowMasking(i, rt, m, &s) * (1 - F);
		Ft_num *= ::abs(o_dot_m * i_dot_m);

		math::real Ft_div = (i_dot_m + (1 / ior) * ::abs(o_dot_m));
		Ft_div *= Ft_div;
		Ft_div *= (cosThetaO * cosThetaI);

		math::Vector transmitance = math::loadScalar(Ft_num / Ft_div);

		// Free all memory
		m_distribution->destroySessionMemory(&s, stackAllocator);

		f = math::mul(transmitance, math::loadScalar((ni * ni) / (no * no)));
	}

	*pdf = (rPdf + tPdf) / (math::real)2.0;
	return f;
}

manta::math::real manta::MicrofacetGlassBSDF::calculatePDF(const IntersectionPoint *surfaceInteraction, const math::Vector &i, const math::Vector &o, StackAllocator *stackAllocator) const {
	return math::real();
}
