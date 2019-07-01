#include <node_output.h>

manta::NodeOutput::NodeOutput(const NodeType *singleType) {
	m_singleType = singleType; 
	m_dimensionsEvaluated = false;
	m_interface = nullptr;
}

manta::NodeOutput::~NodeOutput() {
	/* void */
}

void manta::NodeOutput::initialize() {
	m_interface = generateInterface();
}
