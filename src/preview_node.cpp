#include "../include/preview_node.h"

#include "../include/vector_node_output.h"

manta::PreviewNode::PreviewNode() {
    m_titleInput = nullptr;
    m_dataInput = nullptr;
    m_title = "";
}

manta::PreviewNode::~PreviewNode() {
    /* void */
}

manta::VectorNodeOutput *manta::PreviewNode::getOutput() {
    return static_cast<VectorNodeOutput *>(m_dataInput);
}

void manta::PreviewNode::_initialize() {
    /* void */
}

void manta::PreviewNode::_evaluate() {
    m_titleInput->fullCompute(reinterpret_cast<void *>(&m_title));

    Session::get().registerPreview(this);
}

void manta::PreviewNode::_destroy() {
    Session::get().deregisterPreview(this);
}

void manta::PreviewNode::registerOutputs() {
    /* void */
}

void manta::PreviewNode::registerInputs() {
    registerInput(&m_titleInput, "title");
    registerInput(&m_dataInput, "data");
}
