#include "../include/intersection_point.h"

#include "../include/mesh.h"

void manta::IntersectionPoint::calculateCachedValues() {
    // Generate basis vectors
    m_vertexU = (abs(math::getX(m_vertexNormal)) < 0.1f)
        ? math::constants::XAxis
        : math::constants::YAxis;
    
    m_vertexU = math::normalize(math::cross(m_vertexU, m_vertexNormal));
    m_vertexV = math::cross(m_vertexNormal, m_vertexU);
}

void manta::IntersectionPoint::calculatePartialDerivatives() {
    const AuxFaceData *auxFace = m_mesh->getAuxFace(m_faceIndex);
    const Face *face = m_mesh->getFace(m_faceIndex);
    const math::Vector p0 = *m_mesh->getVertex(face->u);
    const math::Vector p1 = *m_mesh->getVertex(face->v);
    const math::Vector p2 = *m_mesh->getVertex(face->w);

    const math::Vector basis_u = math::normalize(math::sub(p1, p0));
    const math::Vector basis_v = math::cross(m_faceNormal, basis_u);

    const math::Vector p1p0 = math::sub(p1, p0);
    const math::Vector p2p0 = math::sub(p2, p0);

    const math::real u0 = (math::real)0.0;
    const math::real u1 = math::getScalar(math::dot(p1p0, basis_u));
    const math::real u2 = math::getScalar(math::dot(p2p0, basis_u));

    const math::real v0 = (math::real)0.0;
    const math::real v1 = math::getScalar(math::dot(p1p0, basis_v));
    const math::real v2 = math::getScalar(math::dot(p2p0, basis_v));

    const math::real det = (u1 * v2) - (v1 * u2);
    const math::Vector inv_det = math::loadScalar(1 / det);
    const math::Vector inv_00 = math::loadScalar(v2);
    const math::Vector inv_10 = math::loadScalar(-v1);
    const math::Vector inv_01 = math::loadScalar(-u2);
    const math::Vector inv_11 = math::loadScalar(u1);

    // Texture partial derivatives
    math::Vector t0 = math::constants::Zero;
    math::Vector dtdu = math::constants::Zero;
    math::Vector dtdv = math::constants::Zero;

    if (m_mesh->getUseTexCoords()) {
        t0 = *m_mesh->getTexCoord(auxFace->data[0].t);
        const math::Vector t1 = *m_mesh->getTexCoord(auxFace->data[1].t);
        const math::Vector t2 = *m_mesh->getTexCoord(auxFace->data[2].t);

        const math::Vector t1t0 = math::sub(t1, t0);
        const math::Vector t2t0 = math::sub(t2, t0);

        dtdu = math::mul(
            inv_det,
            math::add(math::mul(inv_00, t1t0), math::mul(inv_10, t2t0)));

        dtdv = math::mul(
            inv_det,
            math::add(math::mul(inv_01, t1t0), math::mul(inv_11, t2t0)));
    }

    // Normal partial derivatives
    math::Vector n0 = math::constants::Zero;
    math::Vector dndu = math::constants::Zero;
    math::Vector dndv = math::constants::Zero;

    if (m_mesh->getPerVertexNormals()) {
        n0 = *m_mesh->getNormal(auxFace->data[0].n);
        const math::Vector n1 = *m_mesh->getNormal(auxFace->data[1].n);
        const math::Vector n2 = *m_mesh->getNormal(auxFace->data[2].n);

        const math::Vector n1n0 = math::sub(n1, n0);
        const math::Vector n2n0 = math::sub(n2, n0);

        dndu = math::mul(
            inv_det,
            math::add(math::mul(inv_00, n1n0), math::mul(inv_10, n2n0)));

        dndv = math::mul(
            inv_det,
            math::add(math::mul(inv_01, n1n0), math::mul(inv_11, n2n0)));
    }

    // Parametric coordinates
    const math::Vector pp0 = math::sub(m_position, p0);
    const math::real u = math::getScalar(math::dot(pp0, basis_u));
    const math::real v = math::getScalar(math::dot(pp0, basis_v));

    this->u_basis = basis_u;
    this->v_basis = basis_v;

    this->p0 = p0;
    this->n0 = n0;
    this->t0 = t0;

    this->u = u;
    this->v = v;

    this->dtdu = dtdu;
    this->dtdv = dtdv;
    this->dndu = dndu;
    this->dndv = dndv;
}

void manta::IntersectionPoint::offset(math::real du_s, math::real dv_s) {
    const math::Vector du = math::loadScalar(du_s);
    const math::Vector dv = math::loadScalar(dv_s);

    const math::Vector dpdu = math::normalize(math::cross(this->v_basis, m_vertexNormal));
    const math::Vector dpdv = math::normalize(math::cross(m_vertexNormal, this->u_basis));

    m_position = math::add(
        m_position,
        math::add(math::mul(du, dpdu), math::mul(dv, dpdv)));

    m_vertexNormal = math::normalize(math::add(
        m_vertexNormal,
        math::add(math::mul(du, this->dndu), math::mul(dv, this->dndv))));

    m_textureCoodinates = math::add(
        m_textureCoodinates,
        math::add(math::mul(du, this->dtdu), math::mul(dv, this->dtdv)));
}

manta::math::Vector manta::IntersectionPoint::worldToLocal(const math::Vector &i) const {
    return math::loadVector(
        math::getScalar(math::dot(i, m_vertexU)),
        math::getScalar(math::dot(i, m_vertexV)),
        math::getScalar(math::dot(i, m_vertexNormal)));
}

bool manta::IntersectionPoint::isReflection(const math::Vector &i, const math::Vector &o) const {
    return math::getScalar(math::mul(
        math::dot(i, m_faceNormal),
        math::dot(i, m_vertexNormal)
    )) > 0;
}
