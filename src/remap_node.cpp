#include <remap_node.h>

#include <assert.h>

manta::RemapNode::RemapNode() {
	m_start = math::constants::One;
	m_end = math::constants::Zero;

	m_inputNode = nullptr;
}

manta::RemapNode::RemapNode(const math::Vector &start, const math::Vector &end, const VectorNodeOutput *inputNode) {
	m_start = start;
	m_end = end;

	m_inputNode = inputNode;

	m_output.setEnd(m_end);
	m_output.setStart(m_start);
	m_output.setInput(m_inputNode);
}

manta::RemapNode::~RemapNode() {
	/* void */
}

void manta::RemapNode::_evaluate() {
	m_output.setEnd(m_end);
	m_output.setStart(m_start);
	m_output.setInput(m_inputNode);
}

void manta::RemapNode::registerInputs() {
	registerInput((const NodeOutput **)&m_inputNode, "Input");
}

void manta::RemapNode::registerOutputs() {
	registerOutput(&m_output, "Output");
}
