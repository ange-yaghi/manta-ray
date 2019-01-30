#include <bsdf.h>

manta::BSDF::BSDF() {
	m_mediaInterface = nullptr;
}

manta::BSDF::~BSDF() {

}

manta::math::real manta::BSDF::bidirectionalShadowMasking(const math::Vector &normal, const math::Vector &incident, const math::Vector &o, const math::Vector &m) const {
	return smithBidirectionalShadowMasking(normal, incident, o, m);
}

manta::MediaInterface::DIRECTION manta::BSDF::decideDirection(const math::Vector &incident, const math::Vector &m, MediaInterface::DIRECTION direction) const {
	math::real r1 = math::uniformRandom();

	math::real F = (math::real)1.0;
	if (m_mediaInterface != nullptr) {
		F = m_mediaInterface->fresnelTerm(incident, m, direction);
	}

	if (r1 <= F) {
		if (direction == MediaInterface::DIRECTION_IN) return MediaInterface::DIRECTION_OUT;
		else return MediaInterface::DIRECTION_IN;
	}
	else {
		if (direction == MediaInterface::DIRECTION_IN) return MediaInterface::DIRECTION_IN;
		else return MediaInterface::DIRECTION_OUT;
	}
}

manta::math::Vector manta::BSDF::transmissionDirection(math::real ior, const math::Vector &incident, const math::Vector &m, const math::Vector &n) const {
	math::real sgn = ::signbit(math::getScalar(math::dot(incident, n))) ? -1.0 : 1.0;
	math::real c = math::getScalar(math::dot(incident, m));
	math::real t1 = ior * c - sgn * ::sqrt(1 + ior * (c * c - 1));

	return math::sub(
		math::mul(math::loadScalar(t1), m),
		math::mul(math::loadScalar(ior), incident));
}

manta::math::Vector manta::BSDF::reflectionDirection(const math::Vector &incident, const math::Vector &m) const {
	return math::reflect(incident, m);
}

manta::math::real manta::BSDF::smithBidirectionalShadowMasking(const math::Vector &normal, const math::Vector &incident, const math::Vector &o, const math::Vector &m) const {
	return g1(normal, incident, incident, m) * g1(normal, incident, o, m);
}
