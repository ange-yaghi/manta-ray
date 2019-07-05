#include <microfacet_distribution.h>

#include <stack_allocator.h>

manta::MicrofacetDistribution::MicrofacetDistribution() {
	m_output.setDistribution(this);
}

manta::MicrofacetDistribution::~MicrofacetDistribution() {
	/* void */
}

manta::math::real manta::MicrofacetDistribution::calculatePDF(
		const math::Vector &m, NodeSessionMemory *mem) const {
	math::real cos_theta_m = math::getZ(m);

	return calculateDistribution(m, mem) * cos_theta_m;
}

manta::math::real manta::MicrofacetDistribution::bidirectionalShadowMasking(
		const math::Vector &i, const math::Vector &o, 
		const math::Vector &m, NodeSessionMemory *mem) const {
	return smithBidirectionalShadowMasking(i, o, m, mem);
}

manta::math::real manta::MicrofacetDistribution::smithBidirectionalShadowMasking(
		const math::Vector &i, const math::Vector &o, 
		const math::Vector &m, NodeSessionMemory *mem) const {
	return calculateG1(i, m, mem) * calculateG1(o, m, mem);
}

void manta::MicrofacetDistribution::registerOutputs() {
	setPrimaryOutput(&m_output);
}
