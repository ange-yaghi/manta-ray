#include <phong_bsdf.h>

#include <vector_material_node.h>
#include <assert.h>

manta::PhongBSDF::PhongBSDF() {
	m_powerNode = nullptr;
	m_power = (math::real)1.0;
}

manta::PhongBSDF::~PhongBSDF() {

}

void manta::PhongBSDF::initialize(BSDFInput *bsdfInput, StackAllocator *s) const {
	BSDF::initialize(bsdfInput, s);

	PhongMemory *memory = reinterpret_cast<PhongMemory *>((void *)bsdfInput->cachedParameters);

	if (m_powerNode != nullptr) {
		// Sample the power input and save it in the state container
		math::real power = math::getScalar(m_powerNode->sample(bsdfInput->surfaceInteraction));
		memory->power = power * m_power;

		if (memory->power < (math::real)2.0) memory->power = (math::real)2.0;
	}
	else {
		memory->power = m_power;
	}
}

manta::math::Vector manta::PhongBSDF::generateMicrosurfaceNormal(const BSDFInput &bsdfInput) const {
	PhongMemory *memory = reinterpret_cast<PhongMemory *>((void *)bsdfInput.cachedParameters);

	math::real r1 = math::uniformRandom();
	math::real r2 = math::uniformRandom();

	math::real rho_m = 2 * math::constants::PI * r2;
	math::real cos_theta_m = ::pow(r1, (math::real)1.0 / (memory->power + (math::real)2.0));
	math::real sin_theta_m = ::sqrt((math::real)1.0 - cos_theta_m * cos_theta_m);

	assert(!std::isnan(cos_theta_m));
	assert(!std::isnan(sin_theta_m));

	math::real x = sin_theta_m * ::cos(rho_m);
	math::real y = sin_theta_m * ::sin(rho_m);
	math::real z = cos_theta_m;

	return math::add(math::add(
		math::mul(math::loadScalar(x), bsdfInput.u),
		math::mul(math::loadScalar(y), bsdfInput.v)),
		math::mul(math::loadScalar(z), bsdfInput.normal));
}

manta::math::real manta::PhongBSDF::generateWeight(const BSDFInput &bsdfInput, const math::Vector &m, const math::Vector &o) const {
	math::real i_dot_m = ::abs(math::getScalar(math::dot(bsdfInput.incident, m)));
	if (i_dot_m < 1E-6) return (math::real)0.0;

	math::real g = bidirectionalShadowMasking(bsdfInput, o, m);

	math::real i_dot_n = math::getScalar(math::dot(bsdfInput.incident, bsdfInput.normal));
	math::real m_dot_n = math::getScalar(math::dot(m, bsdfInput.normal));

	math::real num = i_dot_m * g;
	math::real div = ::abs(i_dot_n) * ::abs(m_dot_n);

	if (div < 1E-6) {
		return (math::real)0.0;
	}

	return (num / div);
}

manta::math::real manta::PhongBSDF::g1(const BSDFInput &bsdfInput, const math::Vector &v, const math::Vector &m) const {
	const PhongMemory *memory = reinterpret_cast<const PhongMemory *>((void *)bsdfInput.cachedParameters);

	math::Vector m_dot_v = math::dot(m, v);
	
	if (math::getScalar(math::div(m_dot_v, math::dot(v, bsdfInput.normal))) < 0.0) {
		return (math::real)0.0;
	}

	math::real m_dot_v_s = math::getScalar(m_dot_v);
	math::real m_dot_v_s_2 = m_dot_v_s * m_dot_v_s;

	if (m_dot_v_s_2 >= (math::real)1.0) return 1.0;

	math::real a = ::sqrt(((math::real)0.5 * memory->power + 1) / ((math::real)1.0 - m_dot_v_s * m_dot_v_s)) * m_dot_v_s;
	a = ::abs(a);

	if (a < (math::real)0.0) {
		a = (math::real)0.0;
	}

	math::real secondTerm = (math::real)1.0;
	if (a < (math::real)1.6) {
		secondTerm = (math::real)((3.535 * a + 2.181 * a * a) / (1 + 2.767 * a + 2.577 * a * a));
	}

	return secondTerm;
}
