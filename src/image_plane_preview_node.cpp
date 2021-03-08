#include "../include/image_plane_preview_node.h"

manta::ImagePlanePreviewNode::ImagePlanePreviewNode() {
    m_titleInput = nullptr;
}

manta::ImagePlanePreviewNode::~ImagePlanePreviewNode() {
    /* void */
}

void manta::ImagePlanePreviewNode::_initialize() {
    /* void */
}

void manta::ImagePlanePreviewNode::_evaluate() {
    piranha::native_string title;
    m_titleInput->fullCompute(reinterpret_cast<void *>(&title));

    m_output.setReference(Session::get().createImagePlanePreview(title));
}

void manta::ImagePlanePreviewNode::_destroy() {
    /* void */
}

void manta::ImagePlanePreviewNode::registerOutputs() {
    ObjectReferenceNode::registerOutputs();
}

void manta::ImagePlanePreviewNode::registerInputs() {
    registerInput(&m_titleInput, "title");
}
