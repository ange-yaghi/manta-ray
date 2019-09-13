#include "../include/constructed_vector_node.h"

#include "../include/cached_vector_node.h"

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

piranha::Node *manta::ConstructedVectorNode::_optimize() {
    addFlag(piranha::Node::META_ACTIONLESS);

    piranha::pNodeInput x, y, z, w;
    x = *m_output.getXConnection();
    y = *m_output.getYConnection();
    z = *m_output.getZConnection();
    w = *m_output.getWConnection();

    bool isConstant =
        x->getParentNode()->hasFlag(piranha::Node::META_CONSTANT) &&
        y->getParentNode()->hasFlag(piranha::Node::META_CONSTANT) &&
        z->getParentNode()->hasFlag(piranha::Node::META_CONSTANT) &&
        w->getParentNode()->hasFlag(piranha::Node::META_CONSTANT);

    if (isConstant) {
        addFlag(piranha::Node::META_CONSTANT);

        bool result = evaluate();
        if (!result) return nullptr;

        math::Vector constantValue;
        m_output.sample(nullptr, (void *)&constantValue);

        CachedVectorNode *cached = new CachedVectorNode(constantValue);
        mapOptimizedPort(cached, "__out", "__out");

        return cached;
    }

    return this;
}

void manta::ConstructedVectorNode::registerOutputs() {
    setPrimaryOutput("__out");

    registerOutput(&m_output, "__out");
}

void manta::ConstructedVectorNode::registerInputs() {
    registerInput(m_output.getXConnection(), "x");
    registerInput(m_output.getYConnection(), "y");
    registerInput(m_output.getZConnection(), "z");
    registerInput(m_output.getWConnection(), "w");
}
