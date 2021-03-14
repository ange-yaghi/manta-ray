#include "../include/lens.h"

#include "../include/vector_node_output.h"
#include "../include/image_plane.h"

manta::Lens::Lens() {
    m_aperture = nullptr;
    m_imagePlane = nullptr;
    m_imagePlaneInput = nullptr;
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

    m_imagePlane = getObject<ImagePlane>(m_imagePlaneInput);
    m_sensorResolutionX = m_imagePlane->getWidth();
    m_sensorResolutionY = m_imagePlane->getHeight();

    piranha::native_float sensorWidth, sensorHeight, radius;
    m_radiusInput->fullCompute((void *)&radius);
    m_sensorHeightInput->fullCompute((void *)&sensorHeight);
    sensorWidth = (m_sensorResolutionX / (piranha::native_float)m_sensorResolutionY) * sensorHeight;

    m_sensorHeight = (math::real)sensorHeight;
    m_sensorWidth = (math::real)sensorWidth;
    m_radius = (math::real)radius;

    m_sensorResolutionX = m_sensorResolutionX;
    m_sensorResolutionY = m_sensorResolutionY;

    m_aperture = getObject<Aperture>(m_apertureInput);

    configure();
    update();

    setOutput(this);
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
    registerInput(&m_imagePlaneInput, "image_plane");
    registerInput(&m_apertureInput, "aperture");
}

void manta::Lens::registerOutputs() {
    ObjectReferenceNode<Lens>::registerOutputs();
}
