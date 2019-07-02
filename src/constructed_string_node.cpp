#include <constructed_string_node.h>

manta::ConstructedStringNode::ConstructedStringNode() {
	/* void */
}

manta::ConstructedStringNode::~ConstructedStringNode() {
	/* void */
}

void manta::ConstructedStringNode::_initialize() {
	m_output.initialize();
}

void manta::ConstructedStringNode::_evaluate() {
	/* void */
}

void manta::ConstructedStringNode::_destroy() {
	/* void */
}

void manta::ConstructedStringNode::registerOutputs() {
	setPrimaryOutput(&m_output);
}

void manta::ConstructedStringNode::registerInputs() {
	registerInput(m_output.getStringConnection(), "__in");
}
