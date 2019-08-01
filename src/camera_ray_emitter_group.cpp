#include "../include/camera_ray_emitter_group.h"

#include "../include/vector_node_output.h"

manta::CameraRayEmitterGroup::CameraRayEmitterGroup() {
    m_upInput = nullptr;
    m_positionInput = nullptr;
    m_directionInput = nullptr;
    m_resolutionXInput = nullptr;
    m_resolutionYInput = nullptr;
    m_planeHeightInput = nullptr;
    m_planeDistanceInput = nullptr;
    m_sampleInput = nullptr;
    m_samplerInput = nullptr;
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

    static_cast<piranha::IntValueOutput *>(m_resolutionXInput)->fullCompute((void *)&m_resolutionX);
    static_cast<piranha::IntValueOutput *>(m_resolutionYInput)->fullCompute((void *)&m_resolutionY);

    static_cast<piranha::FloatValueOutput *>(m_planeHeightInput)->fullCompute((void *)&m_planeHeight);
    static_cast<piranha::FloatValueOutput *>(m_planeDistanceInput)->fullCompute((void *)&m_planeDistance);

    static_cast<piranha::IntValueOutput *>(m_sampleInput)->fullCompute((void *)&m_samples);
    m_sampler = static_cast<ObjectReferenceNodeOutput<Sampler2D> *>(m_samplerInput)->getReference();

    m_output.setReference(this);
}

void manta::CameraRayEmitterGroup::_initialize() {
    /* void */
}

void manta::CameraRayEmitterGroup::registerInputs() {
    registerInput(&m_upInput, "up");
    registerInput(&m_positionInput, "position");
    registerInput(&m_directionInput, "direction");
    registerInput(&m_resolutionXInput, "resolution_x");
    registerInput(&m_resolutionYInput, "resolution_y");
    registerInput(&m_planeHeightInput, "plane_height");
    registerInput(&m_planeDistanceInput, "plane_distance");
    registerInput(&m_sampleInput, "samples");
    registerInput(&m_samplerInput, "sampler");
}
