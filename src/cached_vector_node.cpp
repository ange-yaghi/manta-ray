#include "..\include\cached_vector_node.h"

manta::CachedVectorNode::CachedVectorNode() {
    /* void */
}

manta::CachedVectorNode::CachedVectorNode(const math::Vector &v) {
    setValue(v);
}

manta::CachedVectorNode::~CachedVectorNode() {
    /* void */
}

void manta::CachedVectorNode::_initialize() {
    /* void */
}

void manta::CachedVectorNode::_evaluate() {
    /* void */
}

void manta::CachedVectorNode::_destroy() {
    /* void */
}

piranha::Node *manta::CachedVectorNode::_optimize() {
    addFlag(piranha::Node::META_CONSTANT);
    addFlag(piranha::Node::META_ACTIONLESS);

    return this;
}

void manta::CachedVectorNode::registerOutputs() {
    setPrimaryOutput("__out");
    registerOutput(&m_output, "__out");
}
