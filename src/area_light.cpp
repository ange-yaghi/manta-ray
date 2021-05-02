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
    math::real *pdf,
    math::real *depth) const
{
    const math::real offset_u = (u.x * (math::real)2.0 - (math::real)1.0) * m_width / (math::real)2.0;
    const math::real offset_v = (u.y * (math::real)2.0 - (math::real)1.0) * m_height / (math::real)2.0;

    const math::Vector right = math::normalize(math::cross(m_direction, m_up));

    math::Vector pos = math::add(m_origin, math::mul(m_up, math::loadScalar(offset_v)));
    pos = math::add(pos, math::mul(right, math::loadScalar(offset_u)));

    *depth = math::getScalar(math::magnitude(math::sub(pos, ref.m_position)));
    *wi = math::normalize(math::sub(pos, ref.m_position));
    *pdf = pdfIncoming(ref.m_position, pos, *wi);

    return m_intensity;
}

manta::math::real manta::AreaLight::pdfIncoming(const IntersectionPoint &ref, const math::Vector &wi) const {
    math::real depth = math::constants::REAL_MAX;
    const bool intersects = intersect(ref.m_position, wi, &depth);

    if (!intersects) return 0;
    else {
        const math::Vector i = math::add(
            ref.m_position,
            math::mul(math::loadScalar(depth), wi)
        );

        return pdfIncoming(ref.m_position, i, wi);
    }
}

manta::math::Vector manta::AreaLight::L(const IntersectionPoint &ref, const math::Vector &wi) const {
    return m_intensity;
}

bool manta::AreaLight::intersect(const math::Vector &src, const math::Vector &dir_, math::real *depth) const {
    const math::Vector dir = dir_;
    const math::real d_dot_n = math::getScalar(math::dot(dir, m_direction));
    if (d_dot_n == 0) {
        return false;
    }

    const math::Vector d = math::div(
        math::dot(math::sub(m_origin, src), m_direction),
        math::dot(dir, m_direction)
    );

    const math::real intersectionDepth = math::getScalar(d);
    if (intersectionDepth > *depth) return false;
    if (intersectionDepth < 0) return false;

    const math::Vector i = math::add(
        src,
        math::mul(d, dir)
    );

    const math::Vector local_i = math::sub(i, m_origin);

    const math::Vector right = math::normalize(math::cross(m_direction, m_up));
    const math::real u = math::getScalar(math::dot(local_i, right));
    const math::real v = math::getScalar(math::dot(local_i, m_up));

    if (std::abs(u) > m_width / 2) return false;
    else if (std::abs(v) > m_height / 2) return false;
    else {
        *depth = intersectionDepth;
        return true;
    }
}

manta::math::real manta::AreaLight::getArea() const {
    return m_width * m_height;
}

void manta::AreaLight::_evaluate() {
    Light::_evaluate();

    static_cast<VectorNodeOutput *>(m_upInput)->sample(nullptr, &m_up);
    static_cast<VectorNodeOutput *>(m_directionInput)->sample(nullptr, &m_direction);
    static_cast<VectorNodeOutput *>(m_originInput)->sample(nullptr, &m_origin);
    static_cast<VectorNodeOutput *>(m_intensityInput)->sample(nullptr, &m_intensity);

    math::Vector width, height;
    static_cast<VectorNodeOutput *>(m_widthInput)->sample(nullptr, &width);
    static_cast<VectorNodeOutput *>(m_heightInput)->sample(nullptr, &height);

    m_width = math::getScalar(width);
    m_height = math::getScalar(height);
}

void manta::AreaLight::registerInputs() {
    Light::registerInputs();

    registerInput(&m_upInput, "up");
    registerInput(&m_directionInput, "direction");
    registerInput(&m_originInput, "origin");
    registerInput(&m_intensityInput, "intensity");
    registerInput(&m_widthInput, "width");
    registerInput(&m_heightInput, "height");
}

manta::math::real manta::AreaLight::pdfIncoming(const math::Vector &ref, const math::Vector &p, const math::Vector &wi) const {
    const math::real distanceSquared = math::getScalar(math::magnitudeSquared3(math::sub(ref, p)));
    const math::real absDot = math::getScalar(math::abs(math::dot(m_direction, wi)));

    return distanceSquared / (absDot * getArea());
}
