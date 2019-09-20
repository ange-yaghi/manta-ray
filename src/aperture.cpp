#include "../include/aperture.h"

manta::Aperture::Aperture() {
    m_radius = (math::real)m_radius;
}

manta::Aperture::~Aperture() {
    /* void */
}

void manta::Aperture::_initialize() {
    /* void */
}

void manta::Aperture::_evaluate() {
    piranha::native_float radius;
    m_radiusInput->fullCompute((void *)&radius);

    m_radius = (math::real)radius;

    m_output.setReference(this);
}

void manta::Aperture::_destroy() {
    /* void */
}

void manta::Aperture::registerInputs() {
    registerInput(&m_radiusInput, "radius");
}

void manta::Aperture::registerOutputs() {
    ObjectReferenceNode::registerOutputs();
}
