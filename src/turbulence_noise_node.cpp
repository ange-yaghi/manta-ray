#include "../include/turbulence_noise_node.h"

manta::TurbulenceNoiseNode::TurbulenceNoiseNode() {
    /* void */
}

manta::TurbulenceNoiseNode::~TurbulenceNoiseNode() {
    /* void */
}

void manta::TurbulenceNoiseNode::_initialize() {
    /* void */
}

void manta::TurbulenceNoiseNode::registerInputs() {
    registerInput(m_output.getInputConnection(), "in");
    registerInput(m_output.getOctavesConnection(), "octaves");
    registerInput(m_output.getOmegaConnection(), "omega");
}

void manta::TurbulenceNoiseNode::registerOutputs() {
    setPrimaryOutput("out");
    registerOutput(&m_output, "out");
}
