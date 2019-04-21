#include <ggx_distribution.h>

#include <vector_node.h>

#include <assert.h>

manta::GgxDistribution::GgxDistribution() {
	m_width = (math::real)1.0;
	m_minMapWidth = (math::real)1.0;
	m_widthNode = nullptr;
}

manta::GgxDistribution::~GgxDistribution() {

}

void manta::GgxDistribution::initializeSessionMemory(const IntersectionPoint *surfaceInteraction, NodeSessionMemory *memory, StackAllocator *stackAllocator) const {
	MicrofacetDistribution::initializeSessionMemory(surfaceInteraction, memory, stackAllocator);

	GgxMemory *phongMemory = reinterpret_cast<GgxMemory *>((void *)memory->memory);

	if (m_widthNode != nullptr) {
		// Sample the power input and save it in the state container
		math::real width = math::getScalar(m_widthNode->sample(surfaceInteraction));
		phongMemory->width = width * (m_width - m_minMapWidth) + m_minMapWidth;
	}
	else {
		phongMemory->width = m_width;
	}
}

manta::math::Vector manta::GgxDistribution::generateMicrosurfaceNormal(NodeSessionMemory *mem) const {
	GgxMemory *memory = reinterpret_cast<GgxMemory *>((void *)mem->memory);

	math::real width = memory->width;

	math::real r1 = math::uniformRandom();
	math::real r2 = math::uniformRandom();

	math::real rho_m = math::constants::TWO_PI * r2;
	math::real theta_m = ::atan(width * ::sqrt(r1) / ::sqrt(1 - r1));

	math::real cos_theta_m = ::cos(theta_m);
	math::real sin_theta_m = ::sin(theta_m);

	math::Vector t1 = math::loadVector(sin_theta_m, sin_theta_m, cos_theta_m);
	math::Vector t2 = math::loadVector(cos(rho_m), sin(rho_m), (math::real)1.0);

	return math::mul(t1, t2);
}

manta::math::real manta::GgxDistribution::calculateDistribution(const math::Vector &m, NodeSessionMemory *mem) const {
	GgxMemory *memory = reinterpret_cast<GgxMemory *>((void *)mem->memory);

	math::real cos_theta_m = math::getZ(m);

	if (cos_theta_m <= 0) return (math::real)0.0;

	math::real cos2_theta_m = cos_theta_m * cos_theta_m;
	math::real tan2_theta_m = (1 - cos2_theta_m) / cos2_theta_m;
	math::real width2 = memory->width * memory->width;

	// Calculate D(m)
	math::real s1 = (width2 + tan2_theta_m) * cos2_theta_m;
	math::real d_m = width2 / (math::constants::PI * s1 * s1);

	return d_m;
}

manta::math::real manta::GgxDistribution::calculateG1(const math::Vector &v, const math::Vector &m, NodeSessionMemory *mem) const {
	GgxMemory *memory = reinterpret_cast<GgxMemory *>((void *)mem->memory);

	math::real v_dot_m = math::getScalar(math::dot(v, m));
	math::real v_dot_n = (math::getZ(v));
	if ((v_dot_m < 0) != (v_dot_n < 0)) return (math::real)0.0;

	math::real cos_theta_v = math::getZ(v);
	if (cos_theta_v == 0) return (math::real)0.0;

	math::real cos2_theta_v = cos_theta_v * cos_theta_v;
	math::real tan2_theta_v = (1 - cos2_theta_v) / cos2_theta_v;

	math::real s1 = 1 + ::sqrt(1 + memory->width * memory->width * tan2_theta_v);

	return (math::real)2.0 / s1;
}
