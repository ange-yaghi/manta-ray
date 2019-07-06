#include <constructed_float_node.h>

manta::ConstructedFloatNode::ConstructedFloatNode() {
	/* void */
}

manta::ConstructedFloatNode::~ConstructedFloatNode() {
	/* void */
}

void manta::ConstructedFloatNode::_initialize() {
	/* void */
}

void manta::ConstructedFloatNode::_evaluate() {
	/* void */
}

void manta::ConstructedFloatNode::_destroy() {
	/* void */
}

void manta::ConstructedFloatNode::registerOutputs() {
	setPrimaryOutput(&m_output);
	registerOutput(&m_output, "$primary");
}

void manta::ConstructedFloatNode::registerInputs() {
	registerInput(m_output.getInputConnection(), "__in");
}
