#include <microfacet_distribution.h>

#include <stack_allocator.h>

manta::MicrofacetDistribution::MicrofacetDistribution() {

}

manta::MicrofacetDistribution::~MicrofacetDistribution() {

}

manta::math::real manta::MicrofacetDistribution::calculatePDF(const math::Vector &m, MaterialNodeMemory *mem) const {
	math::real cos_theta_m = math::getZ(m);

	return calculateDistribution(m, mem) * cos_theta_m;
}

manta::math::real manta::MicrofacetDistribution::bidirectionalShadowMasking(const math::Vector &i, const math::Vector &o, const math::Vector &m, MaterialNodeMemory *mem) const {
	return smithBidirectionalShadowMasking(i, o, m, mem);
}

manta::math::real manta::MicrofacetDistribution::smithBidirectionalShadowMasking(const math::Vector &i, const math::Vector &o, const math::Vector &m, MaterialNodeMemory *mem) const {
	return calculateG1(i, m, mem) * calculateG1(o, m, mem);
}
