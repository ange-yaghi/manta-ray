#include "../include/perlin_noise_node.h"

manta::PerlinNoiseNode::PerlinNoiseNode() {
    /* void */
}

manta::PerlinNoiseNode::~PerlinNoiseNode() {
    /* void */
}

void manta::PerlinNoiseNode::_initialize() {
    /* void */
}

void manta::PerlinNoiseNode::registerInputs() {
    registerInput(m_output.getInputConnection(), "in");
}

void manta::PerlinNoiseNode::registerOutputs() {
    setPrimaryOutput("out");
    registerOutput(&m_output, "out");
}
