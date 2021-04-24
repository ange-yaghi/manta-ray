#include "../include/dielectric_media_interface.h"

#include <algorithm>

manta::DielectricMediaInterface::DielectricMediaInterface() {
    m_iorIncident = (math::real)1.0;
    m_iorTransmitted = (math::real)1.0;

    m_iorIncidentInput = nullptr;
    m_iorTransmittedInput = nullptr;
}

manta::DielectricMediaInterface::~DielectricMediaInterface() {
    /* void */
}

manta::math::real manta::DielectricMediaInterface::fresnelTerm(
    const math::Vector &i,
    const math::Vector &m,
    Direction d) const
{
    math::real ni, no;
    if (d == Direction::In) {
        ni = m_iorIncident;
        no = m_iorTransmitted;
    }
    else if (d == Direction::Out) {
        ni = m_iorTransmitted;
        no = m_iorIncident;
    }

    const math::real n = no / ni;

    const math::real cosThetaI = math::getScalar(math::abs(math::dot(m, i)));
    const math::real sinThetaI = ::sqrt(std::max((math::real)0.0, 1 - cosThetaI * cosThetaI));

    // Total internal reflection
    if (sinThetaI >= (math::real)1.0) {
        return (math::real)1.0;
    }

    const math::real cosThetaT = ::sqrt(std::max((math::real)0.0, 1 - n * n * sinThetaI * sinThetaI));

    const math::real Rparl = ((no * cosThetaI) - (ni * cosThetaT)) /
        ((no * cosThetaI) + (ni * cosThetaT));
    const math::real Rperp = ((ni * cosThetaI) - (no * cosThetaT)) /
        ((ni * cosThetaI) + (no * cosThetaT));

    return (Rparl * Rparl + Rperp * Rperp) / (math::real)2.0;
}

manta::math::real manta::DielectricMediaInterface::fresnelTerm(
    math::real cosThetaI,
    Direction d) const
{
    math::real ni, no;
    if (d == Direction::In) {
        ni = m_iorIncident;
        no = m_iorTransmitted;
    }
    else if (d == Direction::Out) {
        ni = m_iorTransmitted;
        no = m_iorIncident;
    }

    const math::real sinThetaI = ::sqrt(std::max((math::real)0.0, 1 - cosThetaI * cosThetaI));
    const math::real sinThetaT = (ni / no) * sinThetaI;

    // Total internal reflection
    if (sinThetaT >= (math::real)1.0) {
        return (math::real)1.0;
    }

    const math::real cosThetaT = ::sqrt(std::max((math::real)0.0, 1 - sinThetaT * sinThetaT));

    const math::real Rparl = ((no * cosThetaI) - (ni * cosThetaT)) /
        ((no * cosThetaI) + (ni * cosThetaT));
    const math::real Rperp = ((ni * cosThetaI) - (no * cosThetaT)) /
        ((ni * cosThetaI) + (no * cosThetaT));

    return (Rparl * Rparl + Rperp * Rperp) / (math::real)2.0;
}

manta::math::real manta::DielectricMediaInterface::ior(Direction d) const {
    math::real ni, no;
    if (d == Direction::In) {
        ni = m_iorIncident;
        no = m_iorTransmitted;
    }
    else if (d == Direction::Out) {
        ni = m_iorTransmitted;
        no = m_iorIncident;
    }

    return ni / no;
}

manta::math::real manta::DielectricMediaInterface::no(Direction d) const {
    if (d == Direction::In) {
        return m_iorTransmitted;
    }
    else if (d == Direction::Out) {
        return m_iorIncident;
    }

    // Shouldn't happen
    return m_iorTransmitted;
}

manta::math::real manta::DielectricMediaInterface::ni(Direction d) const {
    if (d == Direction::In) {
        return m_iorIncident;
    }
    else if (d == Direction::Out) {
        return m_iorTransmitted;
    }

    // Shouldn't happen
    return m_iorIncident;
}

void manta::DielectricMediaInterface::_evaluate() {
    MediaInterface::_evaluate();

    piranha::native_float iorTransmitted, iorIncident;

    m_iorIncidentInput->fullCompute((void *)&iorIncident);
    m_iorTransmittedInput->fullCompute((void *)&iorTransmitted);

    m_iorIncident = (math::real)iorIncident;
    m_iorTransmitted = (math::real)iorTransmitted;
}

void manta::DielectricMediaInterface::registerInputs() {
    registerInput(&m_iorIncidentInput, "ior_exterior");
    registerInput(&m_iorTransmittedInput, "ior_interior");
}
