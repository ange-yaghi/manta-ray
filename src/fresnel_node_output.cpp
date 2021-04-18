#include "../include/fresnel_node_output.h"

#include "../include/light_ray.h"

#include <cmath>

manta::FresnelNodeOutput::FresnelNodeOutput() {
    m_iorInput = nullptr;
    m_normalInput = nullptr;
}

manta::FresnelNodeOutput::~FresnelNodeOutput() {
    /* void */
}

void manta::FresnelNodeOutput::sample(const IntersectionPoint *surfaceInteraction, void *target_) const {
    math::Vector *target = reinterpret_cast<math::Vector *>(target_);

    math::Vector normal;
    math::Vector ior_v;

    static_cast<VectorNodeOutput *>(m_normalInput)->sample(surfaceInteraction, &normal);
    static_cast<VectorNodeOutput *>(m_iorInput)->sample(surfaceInteraction, &ior_v);

    const math::real ior = math::getScalar(ior_v);
    const math::real no = 1.0;
    const math::real ni = ior;
    const math::real n = no / ni;

    const math::real cosThetaI = math::getScalar(math::abs(math::dot(normal, surfaceInteraction->m_lightRay->getDirection())));
    const math::real sinThetaI = ::sqrt(std::max((math::real)0.0, 1 - cosThetaI * cosThetaI));

    // Total internal reflection
    if (sinThetaI >= (math::real)1.0) {
        *target = math::constants::One;
        return;
    }

    const math::real cosThetaT = ::sqrt(std::max((math::real)0.0, 1 - n * n * sinThetaI * sinThetaI));

    const math::real Rparl = ((no * cosThetaI) - (ni * cosThetaT)) /
        ((no * cosThetaI) + (ni * cosThetaT));
    const math::real Rperp = ((ni * cosThetaI) - (no * cosThetaT)) /
        ((ni * cosThetaI) + (no * cosThetaT));

    *target = math::loadScalar((Rparl * Rparl + Rperp * Rperp) / (math::real)2.0);
}

void manta::FresnelNodeOutput::discreteSample2d(int x, int y, void *target_) const {
    math::Vector *target = reinterpret_cast<math::Vector *>(target_);
    *target = math::constants::One;
}

void manta::FresnelNodeOutput::fullOutput(const void **target) const {
    /* void */
}

void manta::FresnelNodeOutput::registerInputs() {
    registerInput(&m_normalInput);
    registerInput(&m_iorInput);
}
