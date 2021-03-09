#include "../include/image_preview_node.h"

manta::ImagePreviewNode::ImagePreviewNode() {
    m_titleInput = nullptr;
    m_imagePreview = nullptr;
}

manta::ImagePreviewNode::~ImagePreviewNode() {
    /* void */
}

void manta::ImagePreviewNode::_initialize() {
    /* void */
}

void manta::ImagePreviewNode::_evaluate() {
    piranha::native_string title;
    m_titleInput->fullCompute(reinterpret_cast<void *>(&title));

    m_imagePreview = Session::get().createImagePreview(title);
    m_output.setReference(m_imagePreview);
}

void manta::ImagePreviewNode::_destroy() {
    if (m_imagePreview != nullptr) {
        m_imagePreview->finalized = true;
    }
}

void manta::ImagePreviewNode::registerOutputs() {
    ObjectReferenceNode::registerOutputs();
}

void manta::ImagePreviewNode::registerInputs() {
    registerInput(&m_titleInput, "title");
}
