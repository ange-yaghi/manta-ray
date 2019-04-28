#include <remap_node.h>

#include <assert.h>

manta::RemapNode::RemapNode() {
	m_output.setEnd(math::constants::One);
	m_output.setStart(math::constants::Zero);
	m_output.setInput(nullptr);
}

manta::RemapNode::RemapNode(const math::Vector &start, const math::Vector &end, pNodeInput inputNode) {
	m_output.setEnd(end);
	m_output.setStart(start);
	m_output.setInput(inputNode);
}

manta::RemapNode::~RemapNode() {
	/* void */
}

void manta::RemapNode::_evaluate() {
	/* void */
}

void manta::RemapNode::registerInputs() {
	registerInput(m_output.getInputConnection(), "Input");
}

void manta::RemapNode::registerOutputs() {
	registerOutput(&m_output, "Output");
}
