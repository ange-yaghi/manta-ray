#include <microfacet_reflection_bsdf.h>

#include <microfacet_distribution.h>

manta::MicrofacetReflectionBSDF::MicrofacetReflectionBSDF() {
	m_distribution = nullptr;
}

manta::MicrofacetReflectionBSDF::~MicrofacetReflectionBSDF() {
}

void manta::MicrofacetReflectionBSDF::initializeSessionMemory(const IntersectionPoint *surfaceInteraction, NodeSessionMemory *memory, StackAllocator *stackAllocator) const {
	BSDF::initializeSessionMemory(surfaceInteraction, memory, stackAllocator);
}

manta::math::Vector manta::MicrofacetReflectionBSDF::sampleF(const IntersectionPoint *surfaceInteraction, const math::Vector &i, math::Vector *o, math::real *pdf, StackAllocator *stackAllocator) const {
	constexpr math::Vector reflect = { (math::real)-1.0, (math::real)-1.0, (math::real)1.0, (math::real)1.0 };
	constexpr math::real MIN_EPSILON = (math::real)0.0;

	// Allocate required memory
	NodeSessionMemory s;
	m_distribution->initializeSessionMemory(surfaceInteraction, &s, stackAllocator);

	math::Vector m = m_distribution->generateMicrosurfaceNormal(&s);
	math::Vector ri = math::reflect(i, m);
	math::real o_dot_m = math::getScalar(math::dot(ri, m));

	*o = ri;

	math::real cosThetaO = ::abs(math::getZ(*o));
	math::real cosThetaI = ::abs(math::getZ(i));

	if (o_dot_m <= MIN_EPSILON ||
		cosThetaO <= MIN_EPSILON ||
		cosThetaI <= MIN_EPSILON) {
		// Free all memory
		m_distribution->destroySessionMemory(&s, stackAllocator);

		*pdf = (math::real)0.0;
		return math::constants::Zero;
	}

	*pdf = m_distribution->calculatePDF(m, &s) / ::abs(4 * o_dot_m);

	// Calculate reflectivity
	math::real F = (math::real)1.0; // TODO: fresnel calculation goes here

	math::Vector reflectivity = math::loadScalar(
		m_distribution->calculateDistribution(m, &s) * m_distribution->bidirectionalShadowMasking(i, *o, m, &s) * F / (4 * cosThetaI * cosThetaO));

	// Free all memory
	m_distribution->destroySessionMemory(&s, stackAllocator);

	return reflectivity;
}

manta::math::real manta::MicrofacetReflectionBSDF::calculatePDF(const IntersectionPoint *surfaceInteraction, const math::Vector &i, const math::Vector &o, StackAllocator *stackAllocator) const {
	// Allocate required memory
	NodeSessionMemory s;
	m_distribution->initializeSessionMemory(surfaceInteraction, &s, stackAllocator);

	math::Vector wh = math::normalize(math::add(i, o));
	math::real o_dot_wh = math::getScalar(math::dot(wh, o));
	
	math::real pdf = m_distribution->calculatePDF(wh, &s) / ::abs(4 * o_dot_wh);

	// Free all memory
	m_distribution->destroySessionMemory(&s, stackAllocator);

	return pdf;
}
