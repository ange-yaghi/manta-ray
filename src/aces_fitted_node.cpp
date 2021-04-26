#include "../include/aces_fitted_node.h"

manta::ACESFittedNode::ACESFittedNode() {
    /* void */
}

manta::ACESFittedNode::~ACESFittedNode() {
    /* void */
}

void manta::ACESFittedNode::_initialize() {
    /* void */
}

void manta::ACESFittedNode::registerInputs() {
    registerInput(m_output.getColorConnection(), "in");
}

void manta::ACESFittedNode::registerOutputs() {
    registerOutput(&m_output, "out");
    setPrimaryOutput("out");
}
