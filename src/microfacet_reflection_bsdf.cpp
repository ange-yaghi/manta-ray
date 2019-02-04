#include <microfacet_reflection_bsdf.h>

#include <microfacet_distribution.h>

manta::MicrofacetReflectionBSDF::MicrofacetReflectionBSDF() {
	m_distribution = nullptr;
}

manta::MicrofacetReflectionBSDF::~MicrofacetReflectionBSDF() {
}

manta::math::Vector manta::MicrofacetReflectionBSDF::sampleF(const math::Vector &i, math::Vector *o, math::real *pdf) const {
	constexpr math::Vector reflect = { (math::real) -1.0, (math::real) -1.0, (math::real)1.0, (math::real)1.0 };

	math::Vector m = m_distribution->generateMicrosurfaceNormal();
	math::Vector ri = math::reflect(i, m);

	*pdf = m_distribution->calculatePDF(ri, m) / (4 * math::getScalar(math::dot(ri, m)));

	*o = ri;

	// Calculate reflectance
	math::real cosThetaI = math::getZ(i);
	math::real costhetaO = math::getZ(ri);

	math::real F = (math::real)1.0; // TODO: fresnel calculation goes here

	return math::loadScalar(
		m_distribution->calculateDistribution(m) * m_distribution->bidirectionalShadowMasking(i, ri, m) * F / (4 * math::getScalar(math::dot(ri, m))));
}
