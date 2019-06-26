#include <vector_node.h>

#include <node_output.h>

manta::VectorNode::VectorNode() {
	/* void */
}

manta::VectorNode::~VectorNode() {
	/* void */
}

void manta::VectorNode::_initialize() {
	/* void */
}

void manta::VectorNode::_evaluate() {
	/* void */
}

void manta::VectorNode::_destroy() {
	/* void */
}

void manta::VectorNode::registerOutputs() {
	registerOutput(&m_output, "vector");

	registerOutputReference(m_output.getXConnection(), "x");
	registerOutputReference(m_output.getYConnection(), "y");
	registerOutputReference(m_output.getZConnection(), "z");
	registerOutputReference(m_output.getWConnection(), "w");
}

void manta::VectorNode::registerInputs() {
	registerInput(m_output.getXConnection(), "x");
	registerInput(m_output.getYConnection(), "y");
	registerInput(m_output.getZConnection(), "z");
	registerInput(m_output.getWConnection(), "w");
}
