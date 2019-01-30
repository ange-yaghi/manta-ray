#include <bilayer_bsdf.h>

manta::BilayerBSDF::BilayerBSDF() {
	m_upperLayer = nullptr;
	m_lowerLayer = nullptr;
}

manta::BilayerBSDF::~BilayerBSDF() {

}

manta::math::Vector manta::BilayerBSDF::generateMicrosurfaceNormal(const math::Vector &normal, const math::Vector &incident, const math::Vector &u, const math::Vector &v) const {
	// Check the upper layer first
	math::Vector upper_m = m_upperLayer->generateMicrosurfaceNormal(normal, incident, u, v);
	MediaInterface::DIRECTION upperRT = m_upperLayer->decideDirection(incident, upper_m, MediaInterface::DIRECTION_IN);

	if (upperRT == MediaInterface::DIRECTION_OUT) {
		return upper_m;
	}
	else {
		// Ray has penetrated to lower level

	}
}

manta::math::real manta::BilayerBSDF::generateWeight(const math::Vector &n, const math::Vector &i, const math::Vector &m, const math::Vector &o) const {
	return math::real();
}

