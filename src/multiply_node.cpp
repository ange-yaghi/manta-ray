#include <multiply_node.h>

manta::MultiplyNode::MultiplyNode() {
	/* void */
}

manta::MultiplyNode::~MultiplyNode() {
	/* void */
}

void manta::MultiplyNode::_initialize() {
	/* void */
}

void manta::MultiplyNode::registerInputs() {
	registerInput(m_output.getInputAConnection(), "A");
	registerInput(m_output.getInputBConnection(), "B");
}

void manta::MultiplyNode::registerOutputs() {
	registerOutput(&m_output, "Output");
}
