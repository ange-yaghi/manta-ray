#include <microfacet_distribution.h>

manta::MicrofacetDistribution::MicrofacetDistribution() {

}

manta::MicrofacetDistribution::~MicrofacetDistribution() {

}

manta::math::real manta::MicrofacetDistribution::calculatePDF(const math::Vector &o, const math::Vector &m) const {
	math::real cos_theta_m = math::getZ(m);

	return calculateDistribution(m) * cos_theta_m;
}

manta::math::real manta::MicrofacetDistribution::bidirectionalShadowMasking(const math::Vector &i, const math::Vector &o, const math::Vector &m) const {
	return smithBidirectionalShadowMasking(i, o, m);
}

manta::math::real manta::MicrofacetDistribution::smithBidirectionalShadowMasking(const math::Vector &i, const math::Vector &o, const math::Vector &m) const {
	return calculateG1(i, m) * calculateG1(o, m);
}
