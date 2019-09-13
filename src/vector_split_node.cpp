#include "../include/vector_split_node.h"

manta::VectorSplitNode::VectorSplitNode() {
    /* void */
}

manta::VectorSplitNode::~VectorSplitNode() {
    /* void */
}

void manta::VectorSplitNode::_initialize() {
    m_x.setValueIndex(0);
    m_y.setValueIndex(1);
    m_z.setValueIndex(2);
    m_w.setValueIndex(3);
}

void manta::VectorSplitNode::_evaluate() {
    /* void */
}

void manta::VectorSplitNode::_destroy() {
    /* void */
}

piranha::Node *manta::VectorSplitNode::_optimize() {
    addFlag(piranha::Node::META_ACTIONLESS);

    return this;
}

void manta::VectorSplitNode::registerOutputs() {
    setInterfaceInput(&m_input);

    registerOutput(&m_x, "u");
    registerOutput(&m_y, "v");

    registerOutput(&m_x, "x");
    registerOutput(&m_y, "y");
    registerOutput(&m_z, "z");
    registerOutput(&m_w, "w");

    registerOutput(&m_x, "r");
    registerOutput(&m_y, "g");
    registerOutput(&m_z, "b");
    registerOutput(&m_w, "a");
}

void manta::VectorSplitNode::registerInputs() {
    registerInput(&m_input, "__in");
    registerInput(m_x.getInputConnection(), "__in");
    registerInput(m_y.getInputConnection(), "__in");
    registerInput(m_z.getInputConnection(), "__in");
    registerInput(m_w.getInputConnection(), "__in");
}
