#include <custom_node.h>

manta::CustomNode::CustomNode() {
	/* void */
}

manta::CustomNode::~CustomNode() {
	/* void */
}

void manta::CustomNode::addCustomOutput(NodeOutput *output, const std::string &name, bool primary) {
	CustomPort *newPort = new CustomPort();
	*newPort = { output, name, primary };

	m_customOutputs.push_back(newPort);
}

void manta::CustomNode::addCustomInput(const std::string &name) {
	CustomPort *newPort = new CustomPort();
	*newPort = { nullptr, name, false };

	m_customInputs.push_back(newPort);
}

void manta::CustomNode::_initialize() {
	int inputCount = getCustomInputCount();
	for (int i = 0; i < inputCount; i++) {
		m_customInputs[i]->port->initialize();
	}	
	
	int outputCount = getCustomOutputCount();
	for (int i = 0; i < inputCount; i++) {
		m_customOutputs[i]->port->initialize();
	}
}

void manta::CustomNode::registerInputs() {
	int inputCount = getCustomInputCount();
	for (int i = 0; i < inputCount; i++) {
		registerInput(&m_customInputs[i]->port, m_customInputs[i]->name);
	}
}

void manta::CustomNode::registerOutputs() {
	int outputCount = getCustomOutputCount();
	for (int i = 0; i < outputCount; i++) {
		registerOutputReference(&m_customOutputs[i]->port, m_customOutputs[i]->name);

		if (m_customOutputs[i]->primary) {
			setPrimaryOutput(m_customOutputs[i]->port);
		}
	}
}
