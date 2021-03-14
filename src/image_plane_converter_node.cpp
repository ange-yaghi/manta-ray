#include "../include/image_plane_converter_node.h"

#include "../include/image_plane.h"

manta::ImagePlaneConverterNode::ImagePlaneConverterNode() {
    m_imagePlaneInput = nullptr;
    m_imagePlane = nullptr;
    m_target = nullptr;
}

manta::ImagePlaneConverterNode::~ImagePlaneConverterNode() {
    /* void */
}

void manta::ImagePlaneConverterNode::registerInputs() {
    registerInput(&m_imagePlaneInput, "image_plane");
}

void manta::ImagePlaneConverterNode::registerOutputs() {
    registerOutput(&m_textureOutput, "__out");
    setPrimaryOutput("__out");
}

void manta::ImagePlaneConverterNode::_initialize() {
    /* void */
}

void manta::ImagePlaneConverterNode::_evaluate() {
    m_target = new VectorMap2D();

    m_imagePlane = getObject<ImagePlane>(m_imagePlaneInput);
    m_imagePlane->setPreviewTarget(m_target);

    m_target->initialize(m_imagePlane->getWidth(), m_imagePlane->getHeight());

    m_textureOutput.setMap(m_target);
}

void manta::ImagePlaneConverterNode::_destroy() {
    if (m_target != nullptr) {
        m_target->destroy();
        delete m_target;
    }
}
