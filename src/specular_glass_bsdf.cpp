#include "../include/specular_glass_bsdf.h"

manta::SpecularGlassBSDF::SpecularGlassBSDF() {
    m_mediaInterface = nullptr;
    m_mediaInterfaceInput = nullptr;
}

manta::SpecularGlassBSDF::~SpecularGlassBSDF() {
    /* void */
}

manta::math::Vector manta::SpecularGlassBSDF::sampleF(
    const IntersectionPoint *surfaceInteraction,
    const math::Vector2 &u,
    const math::Vector &i,
    math::Vector *o,
    math::real *pdf,
    RayFlags *flags,
    StackAllocator *stackAllocator)
{
    const math::real i_dot_n = math::getZ(i);
    const math::real F = (m_mediaInterface == nullptr)
        ? (math::real)1.0
        : m_mediaInterface->fresnelTerm(i_dot_n, surfaceInteraction->m_direction);
    const math::real ior = (m_mediaInterface == nullptr)
        ? (math::real)1.55
        : m_mediaInterface->ior(surfaceInteraction->m_direction);
    const math::Vector tint = m_tint.sample(surfaceInteraction);

    const math::real cosThetaI = std::abs(math::getZ(i));
    if (cosThetaI == 0) {
        *pdf = 0;
        return math::constants::Zero;
    }

    if (u.x <= F) {
        *o = math::reflect(i, math::constants::ZAxis);

        const math::real cosThetaO = std::abs(math::getZ(*o));
        if (cosThetaO == 0) {
            *pdf = 0;
            return math::constants::Zero;
        }

        const math::Vector f = math::loadScalar(F / cosThetaO);

        *pdf = F;
        *flags = RayFlag::Delta | RayFlag::Reflection;
        return math::mul(f, tint);
    }
    else {
        if (!refract(i, math::constants::ZAxis, ior, o)) {
            *pdf = 0;
            return math::constants::Zero;
        }

        const math::real cosThetaO = std::abs(math::getZ(*o));
        if (cosThetaO == 0) {
            *pdf = 0;
            return math::constants::Zero;
        }

        math::real f_t = (1 - F) / cosThetaO;
        f_t *= (ior * ior);

        *pdf = 1 - F;
        *flags = RayFlag::Delta | RayFlag::Transmission;
        return math::mul(tint, math::loadScalar(f_t));
    }
}

manta::math::Vector manta::SpecularGlassBSDF::f(
    const IntersectionPoint *surfaceInteraction,
    const math::Vector &i,
    const math::Vector &o,
    StackAllocator *stackAllocator)
{
    return math::constants::Zero;
}

manta::math::real manta::SpecularGlassBSDF::pdf(
    const IntersectionPoint *surfaceInteraction,
    const math::Vector &i,
    const math::Vector &o)
{
    return 0;
}

piranha::Node *manta::SpecularGlassBSDF::_optimize(piranha::NodeAllocator *nodeAllocator) {
    m_tint.optimize();
    return this;
}

void manta::SpecularGlassBSDF::_evaluate() {
    BXDF::_evaluate();

    m_mediaInterface = getObject<MediaInterface>(m_mediaInterfaceInput);
}

void manta::SpecularGlassBSDF::registerInputs() {
    BXDF::registerInputs();

    registerInput(&m_mediaInterfaceInput, "media_interface");
    registerInput(m_tint.getPortAddress(), "tint");
}
