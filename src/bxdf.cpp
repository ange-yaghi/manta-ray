#include "../include/bxdf.h"

#include "../include/vector_node_output.h"

manta::BXDF::BXDF() {
    /* void */
}

manta::BXDF::~BXDF() {
    /* void */
}

void manta::BXDF::_evaluate() {
    setOutput(this);
}

void manta::BXDF::registerInputs() {
    registerInput(m_normal.getPortAddress(), "normal");
}

manta::math::Vector manta::BXDF::sampleNormal(const IntersectionPoint *surfaceInteraction) {
    return m_normal.sample(surfaceInteraction);
}

void manta::BXDF::generateBasisVectors(
    const math::Vector &direction,
    const IntersectionPoint *surfaceInteraction,
    math::Vector *u_p,
    math::Vector *v_p,
    math::Vector *w_p)
{
    const math::Vector baseNormal = sampleNormal(surfaceInteraction);
    const math::Vector normal = math::getScalar(math::dot(direction, baseNormal)) > 0
        ? baseNormal
        : math::negate(baseNormal);

    // Generate basis vectors
    math::Vector u = math::constants::YAxis;
    math::Vector v;
    math::Vector w = normal;
    if (abs(math::getX(w)) < 0.1f) {
        u = math::constants::XAxis;
    }
    u = math::normalize(math::cross(u, w));
    v = math::cross(w, u);

    *u_p = u;
    *v_p = v;
    *w_p = w;
}

manta::math::Vector manta::BXDF::transform(
    const math::Vector &direction,
    const math::Vector &u,
    const math::Vector &v,
    const math::Vector &w)
{
    // Transform direction
    return math::loadVector(
        math::getScalar(math::dot(direction, u)),
        math::getScalar(math::dot(direction, v)),
        math::getScalar(math::dot(direction, w)));
}

manta::math::Vector manta::BXDF::inverseTransform(
    const math::Vector &direction,
    const math::Vector &u,
    const math::Vector &v,
    const math::Vector &w)
{
    math::Vector transformed = math::add(
        math::mul(u, math::loadScalar(math::getX(direction))),
        math::mul(v, math::loadScalar(math::getY(direction)))
    );
    transformed = math::add(
        transformed,
        math::mul(math::loadScalar(math::getZ(direction)), w)
    );

    return transformed;
}
