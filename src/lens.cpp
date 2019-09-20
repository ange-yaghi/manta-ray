#include "../include/lens.h"

#include "../include/vector_node_output.h"

manta::Lens::Lens() {
    m_aperture = nullptr;
}

manta::Lens::~Lens() {
    /* void */
}

manta::math::Vector manta::Lens::getSensorRight() const {
    return math::cross(m_direction, m_up);
}

void manta::Lens::_initialize() {
    ObjectReferenceNode<Lens>::_initialize();
}

void manta::Lens::_evaluate() {
    static_cast<VectorNodeOutput *>(m_positionInput)->sample(nullptr, (void *)&m_position);
    static_cast<VectorNodeOutput *>(m_directionInput)->sample(nullptr, (void *)&m_direction);
    static_cast<VectorNodeOutput *>(m_upInput)->sample(nullptr, (void *)&m_up);

    piranha::native_int resolution_x, resolution_y;
    m_resolutionXInput->fullCompute((void *)&resolution_x);
    m_resolutionYInput->fullCompute((void *)&resolution_y);

    piranha::native_float sensorWidth, sensorHeight, radius;
    m_radiusInput->fullCompute((void *)&radius);
    m_sensorHeightInput->fullCompute((void *)&sensorHeight);
    sensorWidth = (resolution_x / (piranha::native_float)resolution_y) * sensorHeight;

    m_sensorHeight = (math::real)sensorHeight;
    m_sensorWidth = (math::real)sensorWidth;
    m_radius = (math::real)radius;

    m_sensorResolutionX = resolution_x;
    m_sensorResolutionY = resolution_y;

    m_aperture = getObject<Aperture>(m_apertureInput);

    configure();
    update();

    m_output.setReference(this);
}

void manta::Lens::_destroy() {
    ObjectReferenceNode<Lens>::_destroy();
}

void manta::Lens::registerInputs() {
    registerInput(&m_positionInput, "position");
    registerInput(&m_directionInput, "direction");
    registerInput(&m_upInput, "up");
    registerInput(&m_radiusInput, "radius");
    registerInput(&m_sensorHeightInput, "sensor_height");
    registerInput(&m_resolutionXInput, "resolution_x");
    registerInput(&m_resolutionYInput, "resolution_y");
    registerInput(&m_apertureInput, "aperture");
}

void manta::Lens::registerOutputs() {
    ObjectReferenceNode<Lens>::registerOutputs();
}
