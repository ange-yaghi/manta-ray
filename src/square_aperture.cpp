#include "../include/square_aperture.h"

manta::SquareAperture::SquareAperture() {
    m_radius = (math::real)0.0;
}

manta::SquareAperture::~SquareAperture() {
    /* void */
}

bool manta::SquareAperture::isNotBlocked(math::real x, math::real y) const {
    return x > -m_radius && x < m_radius &&y > -m_radius && y < m_radius;
}

void manta::SquareAperture::registerInputs() {
    registerInput(&m_dirt, "dirt");
    registerInput(&m_radiusInput, "half_width");
}
