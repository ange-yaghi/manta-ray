#include "../include/fresnel_node.h"

manta::FresnelNode::FresnelNode() {
    /* void */
}

manta::FresnelNode::~FresnelNode() {
    /* void */
}

void manta::FresnelNode::_initialize() {
    /* void */
}

void manta::FresnelNode::registerInputs() {
    registerInput(m_output.getIorConnection(), "ior");
    registerInput(m_output.getNormalConnection(), "normal");
}

void manta::FresnelNode::registerOutputs() {
    setPrimaryOutput("f");
    registerOutput(&m_output, "f");
}
