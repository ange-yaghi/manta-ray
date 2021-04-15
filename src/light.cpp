#include "../include/light.h"

#include "../include/scene.h"

manta::Light::Light() {
    m_nameInput = nullptr;
}

manta::Light::~Light() {
    /* void */
}

manta::math::Vector manta::Light::sampleIncoming(const IntersectionPoint &ref, const math::Vector2 &u, math::Vector *wi, math::real *pdf, math::real *depth) const {
    return math::constants::Zero;
}

manta::math::real manta::Light::pdfIncoming(const IntersectionPoint &ref, const math::Vector &wi) const {
    return 0;
}

manta::math::Vector manta::Light::L(const IntersectionPoint &ref, const math::Vector &wi) const {
    return math::constants::Zero;
}

bool manta::Light::intersect(const math::Vector &src, const math::Vector &dir, math::real *depth) const {
    return false;
}

void manta::Light::_evaluate() {
    piranha::native_string name;
    m_nameInput->fullCompute((void *)&name);

    m_name = name;
    setOutput(this);
}

void manta::Light::registerInputs() {
    registerInput(&m_nameInput, "name");
}
