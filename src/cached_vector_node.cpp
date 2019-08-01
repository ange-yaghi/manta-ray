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

void manta::CachedVectorNode::registerOutputs() {
    setPrimaryOutput(&m_output);
    registerOutput(&m_output, "__out");
}
