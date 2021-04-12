#include "../include/area_light.h"

manta::AreaLight::AreaLight() {
    m_up = math::constants::YAxis;
    m_direction = math::constants::XAxis;
    m_origin = math::constants::Zero;
    m_intensity = math::constants::Zero;

    m_width = (math::real)1.0;
    m_height = (math::real)1.0;
}

manta::AreaLight::~AreaLight() {
    /* void */
}

manta::math::Vector manta::AreaLight::sampleIncoming(
    const IntersectionPoint &ref,
    const math::Vector2 &u,
    math::Vector *wi,
    math::real *pdf) const
{
    const math::real offset_u = (u.x * (math::real)2.0 - (math::real)1.0) * m_width;
    const math::real offset_v = (u.y * (math::real)2.0 - (math::real)1.0) * m_height;

    const math::Vector right = math::normalize(math::cross(m_direction, m_up));

    math::Vector pos = math::add(m_origin, math::mul(m_up, math::loadScalar(offset_v)));
    pos = math::add(pos, math::mul(right, math::loadScalar(offset_u)));

    *wi = math::normalize(math::sub(pos, ref.m_position));
    *pdf = pdfIncoming(ref.m_position, pos, *wi);

    return m_intensity;
}

manta::math::real manta::AreaLight::pdfIncoming(const IntersectionPoint &ref, const math::Vector &wi) const {
    return math::real();
}

manta::math::real manta::AreaLight::getArea() const {
    return m_width * m_height;
}

manta::math::real manta::AreaLight::pdfIncoming(const math::Vector &ref, const math::Vector &p, const math::Vector &wi) const {
    const math::real distanceSquared = math::getScalar(math::magnitudeSquared3(math::sub(ref, p)));
    const math::real absDot = math::getScalar(math::abs(math::dot(m_direction, wi)));

    return distanceSquared / (absDot * getArea());
}
