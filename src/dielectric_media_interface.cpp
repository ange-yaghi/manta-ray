#include "../include/dielectric_media_interface.h"

#include <algorithm>

manta::DielectricMediaInterface::DielectricMediaInterface() {
    m_iorIncident = (math::real)1.0;
    m_iorTransmitted = (math::real)1.0;
    m_f0 = (math::real)0.0;
    m_f1 = (math::real)1.0;
    m_power = (math::real)1.0;

    m_iorIncidentInput = nullptr;
    m_iorTransmittedInput = nullptr;
    m_f0Input = nullptr;
    m_f1Input = nullptr;
    m_powerInput = nullptr;
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

    math::real F = (Rparl * Rparl + Rperp * Rperp) / (math::real)2.0;
    F = 1 - std::pow(1 - F, m_power);
    return F * m_f1 + (1 - F) * m_f0;
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

    math::real F = (Rparl * Rparl + Rperp * Rperp) / (math::real)2.0;
    F = 1 - std::pow(1 - F, m_power);
    return F * m_f1 + (1 - F) * m_f0;
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
    piranha::native_float f0, f1;
    piranha::native_float power;

    m_iorIncidentInput->fullCompute((void *)&iorIncident);
    m_iorTransmittedInput->fullCompute((void *)&iorTransmitted);
    m_f0Input->fullCompute((void *)&f0);
    m_f1Input->fullCompute((void *)&f1);
    m_powerInput->fullCompute((void *)&power);

    m_iorIncident = (math::real)iorIncident;
    m_iorTransmitted = (math::real)iorTransmitted;
    m_f0 = (math::real)f0;
    m_f1 = (math::real)f1;
    m_power = (math::real)power;
}

void manta::DielectricMediaInterface::registerInputs() {
    registerInput(&m_iorIncidentInput, "ior_exterior");
    registerInput(&m_iorTransmittedInput, "ior_interior");
    registerInput(&m_f0Input, "remap_0");
    registerInput(&m_f1Input, "remap_1");
    registerInput(&m_powerInput, "power");
}
