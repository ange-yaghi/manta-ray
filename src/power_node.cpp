#include <power_node.h>

#include <assert.h>

manta::PowerNode::PowerNode() {
	m_power = (math::real)1.0;

	m_inputNode = nullptr;
}

manta::PowerNode::PowerNode(math::real power, const VectorNode *input) {
	m_power = power;
	m_inputNode = input;
}

manta::PowerNode::~PowerNode() {

}

manta::math::Vector manta::PowerNode::sample(const IntersectionPoint *surfaceInteraction) const {
	assert(m_inputNode != nullptr);

	math::Vector raw = m_inputNode->sample(surfaceInteraction);
	
	math::real x_pow = ::pow(math::getX(raw), m_power);
	math::real y_pow = ::pow(math::getY(raw), m_power);
	math::real z_pow = ::pow(math::getZ(raw), m_power);

	return math::loadVector(x_pow, y_pow, z_pow);
}
