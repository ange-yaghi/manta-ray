#include <phong_emitter.h>

#include <light_ray.h>

manta::PhongEmitter::PhongEmitter() {
	m_samples = 1;
}

manta::PhongEmitter::~PhongEmitter() {
}

void manta::PhongEmitter::generateRays() {
	initializeRays(m_samples);

	math::Vector u = math::constants::YAxis;
	math::Vector v;
	math::Vector w = m_vertexNormal;
	if (abs(math::getX(m_vertexNormal)) < 0.1f) {
		u = math::constants::XAxis;
	}
	u = math::normalize(math::cross(u, m_vertexNormal));
	v = math::cross(m_vertexNormal, u);

	for (int i = 0; i < m_samples; i++) {
		math::Vector m = generateMicrofacet(u, v);
		math::Vector o = reflect(m_incident, m);
		//math::real f = calculateReflectance(o, m);
		math::real w = (math::real)0.0;

		constexpr math::real EPSILON = 1E-5;
		//if (::abs(f) > EPSILON) {
			w = calculateWeight(m, o);
		//}

		if (w > 4.0) {
			w = 4.0;
		}

		if (isnan(w)) {
			int a = 0;
			w = calculateWeight(m, o);
		}
		else if (w == -math::realNAN()) {
			int a = 0;
		}

		LightRay &ray = getRays()[i];
		ray.setDirection(o);
		ray.setSource(getPosition());
		ray.setIntensity(math::constants::Zero);
		ray.setWeight(w);
	}
}

manta::math::real manta::PhongEmitter::g1(const math::Vector &i, const math::Vector &m) const {
	if (math::getScalar(math::div(math::dot(m, i), math::dot(m, m_vertexNormal))) < 0.0) {
		return (math::real)0.0;
	}

	math::real i_dot_n = math::getScalar(math::dot(m_incident, m_vertexNormal));
	math::real power = (1.2 - 0.2 * ::sqrt(::abs(i_dot_n))) * m_power;
	math::real a = sqrt(0.5 * power + 1) / tan(acos(math::getScalar(math::dot(m, i))));

	if (a < (math::real)0.0) {
		a = (math::real)0.0;
	}

	math::real secondTerm = 1.0;
	if (a < (math::real)1.6) {
		secondTerm = (3.535 * a + 2.181 * a * a) / (1 + 2.767 * a + 2.577 * a * a);
	}

	return secondTerm;
}

manta::math::Vector manta::PhongEmitter::generateMicrofacet(const math::Vector &u, const math::Vector &v) const {
	math::real r1 = math::uniformRandom();
	math::real r2 = math::uniformRandom();

	math::real i_dot_n = math::getScalar(math::dot(m_incident, m_vertexNormal));
	math::real power = (1.2 - 0.2 * ::sqrt(::abs(i_dot_n))) * m_power;

	math::real theta_m = ::acos(::pow(r1, (math::real)1.0 / (power + (math::real)2.0)));
	math::real rho_m = 2 * math::constants::PI * r2;

	math::real sin_theta_m = ::sin(theta_m);
	math::real x = sin_theta_m * ::cos(rho_m);
	math::real y = sin_theta_m * ::sin(rho_m);
	math::real z = ::cos(theta_m);

	return math::add(math::add(
		math::mul(math::loadScalar(x), u),
		math::mul(math::loadScalar(y), v)),
		math::mul(math::loadScalar(z), m_vertexNormal));
}

manta::math::real manta::PhongEmitter::calculateDistribution(const math::Vector &m) const {
	math::real m_dot_n = math::getScalar(math::dot(m, m_vertexNormal));
	if (m_dot_n < (math::real)0.0) {
		return (math::real)0.0;
	}
	else {
		return ::pow(m_dot_n, m_power) * (m_power + (math::real)2.0) / (math::constants::TWO_PI);
	}
}

manta::math::real manta::PhongEmitter::calculateG(const math::Vector &o, const math::Vector &m) const {
	return g1(m_incident, m) * g1(o, m);
}

manta::math::real manta::PhongEmitter::calculateFresnel() const {
	return (math::real)(1.0);
}

manta::math::real manta::PhongEmitter::calculateReflectance(const math::Vector &o, const math::Vector &m) const {
	math::Vector hr = math::add(o, m_incident);
	hr = math::normalize(hr);

	math::real F = calculateFresnel();
	math::real G = calculateG(o, hr);
	math::real D = calculateDistribution(hr);

	math::real div = 4 *
		::abs(math::getScalar(math::dot(m, m_incident))) *
		::abs(math::getScalar(math::dot(o, m)));
	return (F * G * D) / div;
}

manta::math::real manta::PhongEmitter::calculateWeight(const math::Vector &m, const math::Vector &o) const {
	math::real g = calculateG(o, m);
	math::real i_dot_m = math::getScalar(math::dot(m_incident, m));
	math::real i_dot_n = math::getScalar(math::dot(m_incident, m_vertexNormal));
	math::real m_dot_n = math::getScalar(math::dot(m, m_vertexNormal));

	math::real num = ::abs(i_dot_m) * g;
	math::real div = ::abs(i_dot_n) * ::abs(m_dot_n);

	if (div < 1E-6) {
		if (num < 1E-6) {
			return (math::real)1.0;
		}
		else {
			return (math::real)0.0;
		}
	}

	return num / div;
}

manta::math::Vector manta::PhongEmitter::reflect(const math::Vector &v, const math::Vector &n) const {
	// r = 2 * n * (v . d) - v
	math::Vector r = math::sub(
		math::mul(
			math::mul(n, math::constants::Double), 
			math::dot(v, n)),
		v);
	return r;
}
