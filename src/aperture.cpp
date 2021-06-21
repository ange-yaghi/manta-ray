#include "../include/aperture.h"

#include "../include/vector_node_output.h"
#include "../include/intersection_point.h"

manta::Aperture::Aperture() {
    m_radius = (math::real)0.0;
    m_radiusInput = nullptr;
    m_dirt = nullptr;
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

    setOutput(this);
}

void manta::Aperture::_destroy() {
    /* void */
}

void manta::Aperture::registerInputs() {
    registerInput(&m_radiusInput, "radius");
    registerInput(&m_dirt, "dirt");
}

void manta::Aperture::registerOutputs() {
    ObjectReferenceNode::registerOutputs();
}

manta::math::real manta::Aperture::sample(math::real x, math::real y) {
    if (m_dirt == nullptr) return 1.0f;

    VectorNodeOutput *dirt = static_cast<VectorNodeOutput *>(m_dirt);
    
    IntersectionPoint p{};
    p.m_textureCoodinates = math::loadVector(0.5f * x + 0.5f, 0.5f * y + 0.5f, 0.0f, 0.0f);

    math::Vector dirtValue;
    dirt->sample(&p, &dirtValue);

    return math::getScalar(dirtValue);
}
