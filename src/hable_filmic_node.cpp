#include "../include/hable_filmic_node.h"

manta::HableFilmicNode::HableFilmicNode() {
    /* void */
}

manta::HableFilmicNode::~HableFilmicNode() {
    /* void */
}

void manta::HableFilmicNode::_initialize() {
    /* void */
}

void manta::HableFilmicNode::registerInputs() {
    registerInput(m_output.getColorConnection(), "in");
    registerInput(m_output.getExposureBiasConnection(), "exposure_bias");
    registerInput(m_output.getWhitePointConnection(), "white_point");
}

void manta::HableFilmicNode::registerOutputs() {
    registerOutput(&m_output, "out");
    setPrimaryOutput("out");
}
