#include "../include/preview_node.h"

#include "../include/vector_node_output.h"

manta::PreviewNode::PreviewNode() {
    m_titleInput = nullptr;
    m_dataInput = nullptr;
    m_refreshRateInput = nullptr;
    m_title = "";
    m_refreshRate = 0.5f;
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
    piranha::native_string title;
    piranha::native_float refreshRate;

    m_titleInput->fullCompute(reinterpret_cast<void *>(&title));
    m_refreshRateInput->fullCompute(reinterpret_cast<void *>(&refreshRate));

    m_title = title;
    m_refreshRate = (math::real)refreshRate;

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
    registerInput(&m_refreshRateInput, "refresh_rate");
}
