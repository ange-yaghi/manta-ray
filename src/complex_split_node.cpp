#include "../include/complex_split_node.h"

manta::ComplexSplitNode::ComplexSplitNode() {
    m_input = nullptr;
}

manta::ComplexSplitNode::~ComplexSplitNode() {
    /* void */
}

void manta::ComplexSplitNode::_initialize() {
    m_r.setValueIndex(0);
    m_i.setValueIndex(1);
}

void manta::ComplexSplitNode::_evaluate() {
    /* void */
}

void manta::ComplexSplitNode::_destroy() {
    /* void */
}

piranha::Node *manta::ComplexSplitNode::_optimize(piranha::NodeAllocator *nodeAllocator) {
    addFlag(piranha::Node::META_ACTIONLESS);

    return this;
}

void manta::ComplexSplitNode::registerOutputs() {
    setInterfaceInput(&m_input);

    registerOutput(&m_r, "r");
    registerOutput(&m_i, "i");
}

void manta::ComplexSplitNode::registerInputs() {
    registerInput(&m_input, "__in");
    registerInput(m_r.getInputConnection(), "__in");
    registerInput(m_i.getInputConnection(), "__in");
}
