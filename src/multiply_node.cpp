#include "../include/multiply_node.h"

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
	registerInput(m_output.getInputAConnection(), "__left");
	registerInput(m_output.getInputBConnection(), "__right");
}

void manta::MultiplyNode::registerOutputs() {
	registerOutput(&m_output, "__out");
}
