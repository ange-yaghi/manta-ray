#include <single_color_node.h>

manta::SingleColorNode::SingleColorNode() {
	/* void */
}

manta::SingleColorNode::SingleColorNode(const math::Vector &color) {
	m_output.setValue(color);
}

manta::SingleColorNode::~SingleColorNode() {
	/* void */
}

void manta::SingleColorNode::_initialize() {
	/* void */
}

void manta::SingleColorNode::_evaluate() {
	/* void */
}

void manta::SingleColorNode::_destroy() {
	/* void */
}

void manta::SingleColorNode::registerOutputs() {
	registerOutput(&m_output, "Color");
}

void manta::SingleColorNode::registerInputs() {
	/* void */
}
