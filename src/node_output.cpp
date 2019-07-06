#include <node_output.h>

#include <node.h>

manta::NodeOutput::NodeOutput(const NodeType *singleType) {
	m_singleType = singleType; 
	m_dimensionsEvaluated = false;
	m_evaluated = false;
	m_interface = nullptr;
}

manta::NodeOutput::~NodeOutput() {
	/* void */
}

void manta::NodeOutput::initialize() {
	m_interface = generateInterface();
	registerInputs();
}

void manta::NodeOutput::evaluate() {
	if (m_evaluated) return;
	m_evaluated = true;

	int inputCount = getInputCount();
	for (int i = 0; i < inputCount; i++) {
		(*m_inputs[i])->evaluate();
	}

	if (m_parentNode != nullptr) {
		m_parentNode->evaluate();
	}
}
