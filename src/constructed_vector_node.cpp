#include "../include/constructed_vector_node.h"

manta::ConstructedVectorNode::ConstructedVectorNode() {
    /* void */
}

manta::ConstructedVectorNode::~ConstructedVectorNode() {
    /* void */
}

void manta::ConstructedVectorNode::_initialize() {
    m_output.initialize();
}

void manta::ConstructedVectorNode::_evaluate() {
    /* void */
}

void manta::ConstructedVectorNode::_destroy() {
    /* void */
}

void manta::ConstructedVectorNode::registerOutputs() {
    setPrimaryOutput(&m_output);

    registerOutput(&m_output, "__out");
}

void manta::ConstructedVectorNode::registerInputs() {
    registerInput(m_output.getXConnection(), "x");
    registerInput(m_output.getYConnection(), "y");
    registerInput(m_output.getZConnection(), "z");
    registerInput(m_output.getWConnection(), "w");
}
