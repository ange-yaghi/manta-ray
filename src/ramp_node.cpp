#include <ramp_node.h>

manta::RampNode::RampNode() {
	/* void */
}

manta::RampNode::~RampNode() {
	/* void */
}

void manta::RampNode::_initialize() {
	/* void */
}

void manta::RampNode::registerInputs() {
	registerInput(m_output.getDcConnection(), "DC");
	registerInput(m_output.getInputConnection(), "Input");
	registerInput(m_output.getFootConnection(), "Foot");
	registerInput(m_output.getSlopeConnection(), "Slope");
}

void manta::RampNode::registerOutputs() {
	registerOutput(&m_output, "Output");
}
