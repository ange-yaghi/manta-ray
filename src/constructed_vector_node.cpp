#include <constructed_vector_node.h>

#include <node_output.h>

manta::ConstructedVectorNode::ConstructedVectorNode() {
	/* void */
}

manta::ConstructedVectorNode::~ConstructedVectorNode() {
	/* void */
}

void manta::ConstructedVectorNode::_initialize() {
	m_output.initialize();
}

void manta::ConstructedVectorNode::_evaluate() {
	/* void */
}

void manta::ConstructedVectorNode::_destroy() {
	/* void */
}

void manta::ConstructedVectorNode::registerOutputs() {
	setPrimaryOutput(&m_output);

	registerOutputReference(m_output.getXConnection(), "x");
	registerOutputReference(m_output.getYConnection(), "y");
	registerOutputReference(m_output.getZConnection(), "z");
	registerOutputReference(m_output.getWConnection(), "w");

	registerOutputReference(m_output.getXConnection(), "r");
	registerOutputReference(m_output.getYConnection(), "g");
	registerOutputReference(m_output.getZConnection(), "b");
	registerOutputReference(m_output.getWConnection(), "a");
}

void manta::ConstructedVectorNode::registerInputs() {
	registerInput(m_output.getXConnection(), "in_x");
	registerInput(m_output.getYConnection(), "in_y");
	registerInput(m_output.getZConnection(), "in_z");
	registerInput(m_output.getWConnection(), "in_w");
}
