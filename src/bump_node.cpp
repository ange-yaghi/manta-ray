#include "../include/bump_node.h"

manta::BumpNode::BumpNode() {
    /* void */
}

manta::BumpNode::~BumpNode() {
    /* void */
}

void manta::BumpNode::_initialize() {
    /* void */
}

void manta::BumpNode::registerInputs() {
    registerInput(m_output.getDisplacementConnection(), "displacement");
    registerInput(m_output.getNormalConnection(), "normal");
}

void manta::BumpNode::registerOutputs() {
    setPrimaryOutput("bump_normal");
    registerOutput(&m_output, "bump_normal");
}
