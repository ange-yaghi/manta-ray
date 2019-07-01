#include <power_node.h>

#include <assert.h>

manta::PowerNode::PowerNode() {
	m_power = (math::real)1.0;
	m_inputNode = nullptr;
	m_powerNode = nullptr;

	m_output.setDefaultPower(m_power);
	m_output.setInput(m_inputNode);
	m_output.setPowerNode(m_powerNode);
}

manta::PowerNode::PowerNode(math::real power, VectorNodeOutput *input) {
	m_power = power;
	m_inputNode = input;
	m_powerNode = nullptr;

	m_output.setDefaultPower(m_power);
	m_output.setInput(m_inputNode);
	m_output.setPowerNode(m_powerNode);
}

manta::PowerNode::~PowerNode() {
	/* void */
}

void manta::PowerNode::_initialize() {
	m_output.setDefaultPower(m_power);
	m_output.setInput(m_inputNode);
	m_output.setPowerNode(m_powerNode);
}

void manta::PowerNode::registerInputs() {
	registerInput((pNodeInput *)&m_inputNode, "Input");
	registerInput((pNodeInput *)&m_powerNode, "Power");
}

void manta::PowerNode::registerOutputs() {
	registerOutput(&m_output, "Output");
}
