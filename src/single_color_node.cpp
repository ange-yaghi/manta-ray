#include <single_color_node.h>

manta::SingleColorNode::SingleColorNode() {
	m_color = math::constants::Zero;
}

manta::SingleColorNode::SingleColorNode(const math::Vector &color) {
	m_color = color;
}

manta::SingleColorNode::~SingleColorNode() {
	/* void */
}

manta::math::Vector manta::SingleColorNode::sample(const IntersectionPoint *) const {
	return m_color;
}
