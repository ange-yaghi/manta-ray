#include <bsdf.h>

manta::BSDF::BSDF() {

}

manta::BSDF::~BSDF() {

}

manta::math::real manta::BSDF::bidirectionalShadowMasking(const math::Vector &normal, const math::Vector &incident, const math::Vector &o, const math::Vector &m) const {
	return smithBidirectionalShadowMasking(normal, incident, o, m);
}

manta::math::real manta::BSDF::smithBidirectionalShadowMasking(const math::Vector &normal, const math::Vector &incident, const math::Vector &o, const math::Vector &m) const {
	return g1(normal, incident, incident, m) * g1(normal, incident, o, m);
}
