#include <phong_distribution.h>

#include <vector_material_node.h>

#include <assert.h>

manta::PhongDistribution::PhongDistribution() {
	m_power = (math::real)1.0;
	m_minMapPower = (math::real)1.0;
	m_powerNode = nullptr;
}

manta::PhongDistribution::~PhongDistribution() {

}

void manta::PhongDistribution::initialize(const IntersectionPoint *surfaceInteraction, MaterialNodeMemory *memory, StackAllocator *stackAllocator) const {
	MicrofacetDistribution::initialize(surfaceInteraction, memory, stackAllocator);

	PhongMemory *phongMemory = reinterpret_cast<PhongMemory *>((void *)memory->memory);

	if (m_powerNode != nullptr) {
		// Sample the power input and save it in the state container
		math::real power = math::getScalar(m_powerNode->sample(surfaceInteraction));
		phongMemory->power = power * (m_power - m_minMapPower) + m_minMapPower;
	}
	else {
		phongMemory->power = m_power;
	}
}

manta::math::Vector manta::PhongDistribution::generateMicrosurfaceNormal(MaterialNodeMemory *mem) const {
	PhongMemory *memory = reinterpret_cast<PhongMemory *>((void *)mem->memory);

	math::real power = memory->power;

	math::real r1 = math::uniformRandom();
	math::real r2 = math::uniformRandom();

	math::real rho_m = math::constants::TWO_PI * r2;
	math::real cos_theta_m = ::pow(r1, (math::real)1.0 / (power + (math::real)2.0));
	math::real sin_theta_m = ::sqrt((math::real)1.0 - cos_theta_m * cos_theta_m);

	assert(!std::isnan(cos_theta_m));
	assert(!std::isnan(sin_theta_m));

	math::Vector t1 = math::loadVector(sin_theta_m, sin_theta_m, cos_theta_m);
	math::Vector t2 = math::loadVector(cos(rho_m), sin(rho_m), (math::real)1.0);

	return math::mul(t1, t2);
}

manta::math::real manta::PhongDistribution::calculateDistribution(const math::Vector &m, MaterialNodeMemory *mem) const {
	PhongMemory *memory = reinterpret_cast<PhongMemory *>((void *)mem->memory);

	math::real cos_theta_m = math::getZ(m);
	if (cos_theta_m <= 0) return (math::real)0.0;

	math::real d_m = ((memory->power + (math::real)2.0) / math::constants::TWO_PI) * ::pow(cos_theta_m, memory->power);
	return d_m;
}

manta::math::real manta::PhongDistribution::calculateG1(const math::Vector &v, const math::Vector &m, MaterialNodeMemory *mem) const {
	PhongMemory *memory = reinterpret_cast<PhongMemory *>((void *)mem->memory);
	
	math::real v_dot_m = (math::getScalar(math::dot(v, m)));
	math::real v_dot_n = (math::getZ(v));
	if ((v_dot_m < 0) != (v_dot_n < 0)) return (math::real)0.0;

	math::real cos_theta_v = ::abs(math::getZ(v));
	math::real sin2_theta_v = 1 - cos_theta_v * cos_theta_v;

	if (sin2_theta_v == 0) return 1.0;

	// tan_theta_v term is split to take advantage of the sqrt already here
	// Full expression: a = sqrt(0.5 * power + 1) / tan_theta_v
	math::real a = ::sqrt(((math::real)0.5 * memory->power + 1) / sin2_theta_v) * cos_theta_v;

	math::real secondTerm = (math::real)1.0;
	if (a < (math::real)1.6) {
		secondTerm = (math::real)((3.535 * a + 2.181 * a * a) / (1 + 2.767 * a + 2.577 * a * a));
	}

	assert(a >= 0);
	assert(secondTerm >= 0);

	return secondTerm;
}
