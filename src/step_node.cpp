#include "../include/step_node.h"

manta::StepNode::StepNode() {
    /* void */
}

manta::StepNode::~StepNode() {
    /* void */
}

void manta::StepNode::_initialize() {
    m_output.initialize();
}

void manta::StepNode::registerInputs() {
    registerInput(m_output.getDcConnection(), "dc");
    registerInput(m_output.getInputConnection(), "s");
    registerInput(m_output.getFootConnection(), "foot");
    registerInput(m_output.getStepConnection(), "step");
}

void manta::StepNode::registerOutputs() {
    setPrimaryOutput("__out");
    registerOutput(&m_output, "__out");
}
