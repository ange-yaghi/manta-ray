#include "../include/filter.h"

manta::Filter::Filter() {
    m_radiusInput = nullptr;
}

manta::Filter::~Filter() {
    /* void */
}

void manta::Filter::_initialize() {
    /* void */
}

void manta::Filter::_evaluate() {
    piranha::native_float radius;
    m_radiusInput->fullCompute((void *)&radius);

    m_extents.x = (math::real)radius;
    m_extents.y = (math::real)radius;

    setOutput(this);
}

void manta::Filter::registerInputs() {
    registerInput(&m_radiusInput, "radius");
}
