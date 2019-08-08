#include "../include/surface_interaction_node.h"

manta::SurfaceInteractionNode::SurfaceInteractionNode() {
    /* void */
}

manta::SurfaceInteractionNode::~SurfaceInteractionNode() {
    /* void */
}

void manta::SurfaceInteractionNode::_initialize() {
    m_normalOutput.initialize();
    m_faceNormalOutput.initialize();
    m_positionOutput.initialize();
    m_textureCoordinatesOutputs.initialize();
    m_depthOutput.initialize();
    m_incidentDirectionOutput.initialize();
    m_incidentSourceOutput.initialize();
}

void manta::SurfaceInteractionNode::registerOutputs() {
    registerOutput(&m_normalOutput, "normal");
    registerOutput(&m_faceNormalOutput, "face_normal");
    registerOutput(&m_positionOutput, "position");
    registerOutput(&m_textureCoordinatesOutputs, "tex_coords");
    registerOutput(&m_depthOutput, "depth");
    registerOutput(&m_incidentDirectionOutput, "light_direction");
    registerOutput(&m_incidentSourceOutput, "light_source");
}
