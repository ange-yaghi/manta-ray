#include <microfacet_transmission_bsdf.h>

#include <microfacet_distribution.h>

manta::MicrofacetTransmissionBSDF::MicrofacetTransmissionBSDF() {
	m_distribution = nullptr;
}

manta::MicrofacetTransmissionBSDF::~MicrofacetTransmissionBSDF() {
}

void manta::MicrofacetTransmissionBSDF::initialize(const IntersectionPoint *surfaceInteraction, MaterialNodeMemory *memory, StackAllocator *stackAllocator) const {
	BSDF::initialize(surfaceInteraction, memory, stackAllocator);
}

manta::math::Vector manta::MicrofacetTransmissionBSDF::sampleF(const IntersectionPoint *surfaceInteraction, const math::Vector &i, math::Vector *o, math::real *pdf, StackAllocator *stackAllocator) const {
	constexpr math::Vector reflect = { (math::real) - 1.0, (math::real) - 1.0, (math::real)1.0, (math::real)1.0 };

	// Allocate required memory
	MaterialNodeMemory s;
	m_distribution->initialize(surfaceInteraction, &s, stackAllocator);

	math::real ior = m_mediaInterface->ior(surfaceInteraction->m_direction);
	math::Vector m = m_distribution->generateMicrosurfaceNormal(&s);
	math::Vector rt;

	if (!refract(i, m, ior, &rt)) {
		// Free all memory
		m_distribution->free(&s, stackAllocator);

		*pdf = (math::real)0.0;
		return math::constants::Zero;
	}

	if ((math::getZ(rt) > 0) == (math::getZ(i) > 0)) {
		// Free all memory
		m_distribution->free(&s, stackAllocator);

		*pdf = (math::real)0.0;

		return math::constants::Zero;
	}

	math::real o_dot_m = math::getScalar(math::dot(rt, m));
	math::real i_dot_m = math::getScalar(math::dot(i, m));

	*o = rt;

	if (i_dot_m <= (math::real)0.0) {
		// Free all memory
		m_distribution->free(&s, stackAllocator);

		*pdf = (math::real)0.0;
		return math::constants::Zero;
	}

	math::real no = m_mediaInterface->no(surfaceInteraction->m_direction);
	math::real ni = m_mediaInterface->ni(surfaceInteraction->m_direction);
	math::real jacobian = no * no * ::abs(o_dot_m);
	math::real jacobian_div = (ni * ::abs(i_dot_m) + no * ::abs(o_dot_m));
	jacobian /= (jacobian_div * jacobian_div);

	*pdf = m_distribution->calculatePDF(m, &s) * jacobian;

	// Calculate transmitance
	math::real cosThetaI = ::abs(math::getZ(i));
	math::real costhetaO = ::abs(math::getZ(rt));

	math::real F = (math::real)0.0; // TODO: fresnel calculation goes here

	math::real Ft_num = ior * ior * m_distribution->calculateDistribution(m, &s) * m_distribution->bidirectionalShadowMasking(i, rt, m, &s) * (1 - F);
	Ft_num *= ::abs(o_dot_m * i_dot_m);

	math::real Ft_div = (i_dot_m + ior * ::abs(o_dot_m));
	Ft_div *= Ft_div;
	Ft_div *= (costhetaO * cosThetaI);

	math::Vector transmitance = math::loadScalar(Ft_num / Ft_div);

	// Free all memory
	m_distribution->free(&s, stackAllocator);

	return transmitance;
}

manta::math::real manta::MicrofacetTransmissionBSDF::calculatePDF(const IntersectionPoint *surfaceInteraction, const math::Vector &i, const math::Vector &o, StackAllocator *stackAllocator) const {
	// TODO
	return (math::real)0.0;
}
