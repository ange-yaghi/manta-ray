#include <srgb_node.h>

#include <node_output.h>

manta::SrgbNode::SrgbNode() {
	/* void */
}

manta::SrgbNode::~SrgbNode() {
	/* void */
}

void manta::SrgbNode::_initialize() {
	m_output.initialize();
}

void manta::SrgbNode::_evaluate() {
	/* void */
}

void manta::SrgbNode::_destroy() {
	/* void */
}

void manta::SrgbNode::registerOutputs() {
	setPrimaryOutput(&m_output);

	registerOutput(&m_output, "color");
}

void manta::SrgbNode::registerInputs() {
	registerInput(m_output.getRConnection(), "r");
	registerInput(m_output.getGConnection(), "g");
	registerInput(m_output.getBConnection(), "b");
	registerInput(m_output.getAConnection(), "a");
}
