#include <step_node.h>

manta::StepNode::StepNode() {
	/* void */
}

manta::StepNode::~StepNode() {
	/* void */
}

void manta::StepNode::_initialize() {
	/* void */
}

void manta::StepNode::registerInputs() {
	registerInput(m_output.getDcConnection(), "DC");
	registerInput(m_output.getInputConnection(), "Input");
	registerInput(m_output.getFootConnection(), "Foot");
	registerInput(m_output.getStepConnection(), "Step");
}

void manta::StepNode::registerOutputs() {
	registerOutput(&m_output, "Output");
}
