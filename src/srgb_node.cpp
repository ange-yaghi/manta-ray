#include "../include/srgb_node.h"

#include "../include/cached_vector_node.h"

manta::SrgbNode::SrgbNode() {
    /* void */
}

manta::SrgbNode::~SrgbNode() {
    /* void */
}

void manta::SrgbNode::_initialize() {
    m_output.initialize();
}

void manta::SrgbNode::_evaluate() {
    /* void */
}

void manta::SrgbNode::_destroy() {
    /* void */
}

void manta::SrgbNode::registerOutputs() {
    setPrimaryOutput("color");

    registerOutput(&m_output, "color");
}

void manta::SrgbNode::registerInputs() {
    registerInput(m_output.getRConnection(), "r");
    registerInput(m_output.getGConnection(), "g");
    registerInput(m_output.getBConnection(), "b");
    registerInput(m_output.getAConnection(), "a");
}

piranha::Node *manta::SrgbNode::_optimize(piranha::NodeAllocator *nodeAllocator) {
    addFlag(piranha::Node::META_ACTIONLESS);

    piranha::pNodeInput r, g, b, a;
    r = *m_output.getRConnection();
    g = *m_output.getGConnection();
    b = *m_output.getBConnection();
    a = *m_output.getAConnection();

    const bool isConstant =
        r->getParentNode()->hasFlag(piranha::Node::META_CONSTANT) &&
        g->getParentNode()->hasFlag(piranha::Node::META_CONSTANT) &&
        b->getParentNode()->hasFlag(piranha::Node::META_CONSTANT) &&
        a->getParentNode()->hasFlag(piranha::Node::META_CONSTANT);

    if (isConstant) {
        addFlag(piranha::Node::META_CONSTANT);

        const bool result = evaluate();
        if (!result) return nullptr;

        math::Vector constantValue;
        m_output.sample(nullptr, (void *)&constantValue);

        CachedVectorNode *cached = nodeAllocator->allocate<CachedVectorNode>();
        cached->setValue(constantValue);
        mapOptimizedPort(cached, "__out", "color");

        return cached;
    }

    return this;
}
