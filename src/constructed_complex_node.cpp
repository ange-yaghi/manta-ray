#include "../include/constructed_complex_node.h"

manta::ConstructedComplexNode::ConstructedComplexNode() {
    /* void */
}

manta::ConstructedComplexNode::~ConstructedComplexNode() {
    /* void */
}

void manta::ConstructedComplexNode::_initialize() {
    m_output.initialize();
}

void manta::ConstructedComplexNode::_evaluate() {
    /* void */
}

void manta::ConstructedComplexNode::_destroy() {
    /* void */
}

void manta::ConstructedComplexNode::registerOutputs() {
    registerOutput(&m_output, "__out");
    setPrimaryOutput("__out");
}

void manta::ConstructedComplexNode::registerInputs() {
    registerInput(m_output.getRConnection(), "r");
    registerInput(m_output.getIConnection(), "i");
}
