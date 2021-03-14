#include "../include/camera_ray_emitter_group.h"

#include "../include/vector_node_output.h"
#include "../include/image_plane.h"

manta::CameraRayEmitterGroup::CameraRayEmitterGroup() {
    m_upInput = nullptr;
    m_positionInput = nullptr;
    m_directionInput = nullptr;
    m_resolutionX = 0;
    m_resolutionY = 0;
    m_planeHeightInput = nullptr;
    m_planeDistanceInput = nullptr;
    m_sampler = nullptr;
    m_imagePlaneInput = nullptr;
    m_imagePlane = nullptr;
}

manta::CameraRayEmitterGroup::~CameraRayEmitterGroup() {
    /* void */
}

void manta::CameraRayEmitterGroup::freeEmitter(CameraRayEmitter *rayEmitter, StackAllocator *stackAllocator) const {
    stackAllocator->free(rayEmitter);
}

void manta::CameraRayEmitterGroup::_evaluate() {
    static_cast<VectorNodeOutput *>(m_upInput)->sample(nullptr, (void *)&m_up);
    static_cast<VectorNodeOutput *>(m_positionInput)->sample(nullptr, (void *)&m_position);
    static_cast<VectorNodeOutput *>(m_directionInput)->sample(nullptr, (void *)&m_direction);

    m_imagePlane = getObject<ImagePlane>(m_imagePlaneInput);

    piranha::native_float planeHeight, planeDistance;
    m_planeHeightInput->fullCompute((void *)&planeHeight);
    m_planeDistanceInput->fullCompute((void *)&planeDistance);

    m_resolutionX = m_imagePlane->getWidth();
    m_resolutionY = m_imagePlane->getHeight();
    m_planeHeight = planeHeight;
    m_planeDistance = planeDistance;

    setOutput(this);
}

void manta::CameraRayEmitterGroup::_initialize() {
    /* void */
}

void manta::CameraRayEmitterGroup::registerInputs() {
    registerInput(&m_upInput, "up");
    registerInput(&m_positionInput, "position");
    registerInput(&m_directionInput, "direction");
    registerInput(&m_planeHeightInput, "plane_height");
    registerInput(&m_planeDistanceInput, "plane_distance");
    registerInput(&m_imagePlaneInput, "image_plane");
}
