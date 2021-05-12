#include "../include/bsdf.h"

#include "../include/bxdf.h"
#include "../include/intersection_point.h"

manta::BSDF::BSDF() : ObjectReferenceNode<BSDF>() {
    m_bxdfCount = 0;
}

manta::BSDF::BSDF(BXDF *bxdf) : ObjectReferenceNode<BSDF>() {
    m_bxdfCount = 0;

    addBxdf(bxdf);
}

manta::BSDF::~BSDF() {
    /* void */
}

manta::math::Vector manta::BSDF::sampleF(
    const IntersectionPoint *surfaceInteraction,
    const math::Vector2 &u,
    const math::Vector &i,
    math::Vector *o,
    math::real *pdf,
    RayFlags *flags,
    StackAllocator *stackAllocator,
    bool cosineWeight) const 
{
    const int bxdf_n = m_bxdfCount;
    const int bxdf_i = std::min((int)std::floor(u.x * bxdf_n), bxdf_n - 1);

    math::Vector2 remapped_u = { u.x * bxdf_n - bxdf_i, u.y };

    BXDF *bxdf = m_bxdfs[bxdf_i];
    const math::Vector normal = bxdf->sampleNormal(surfaceInteraction);

    math::Vector basis_u, basis_v, basis_w;
    bxdf->generateBasisVectors(i, surfaceInteraction, &basis_u, &basis_v, &basis_w);

    const math::Vector i_local = bxdf->transform(i, basis_u, basis_v, basis_w);
    math::Vector o_local;
    math::Vector f = bxdf->sampleF(surfaceInteraction, remapped_u, i_local, &o_local, pdf, flags, stackAllocator);

    if (cosineWeight) {
        f = math::mul(f, math::abs(math::expandZ(o_local)));
    }

    *o = bxdf->inverseTransform(o_local, basis_u, basis_v, basis_w);

    if (*pdf == (math::real)0.0) {
        return math::constants::Zero;
    }

    if (m_bxdfCount > 1) {
        if ((*flags & RayFlag::Delta) == 0) {
            for (int j = 0; j < m_bxdfCount; j++) {
                if (j != bxdf_i) {
                    math::Vector basis_u, basis_v, basis_w;
                    m_bxdfs[j]->generateBasisVectors(i, surfaceInteraction, &basis_u, &basis_v, &basis_w);

                    const math::Vector i_local = m_bxdfs[j]->transform(i, basis_u, basis_v, basis_w);
                    const math::Vector o_local = m_bxdfs[j]->transform(*o, basis_u, basis_v, basis_w);

                    if (math::getZ(i_local) <= 0 || math::getZ(o_local) <= 0) {
                        continue;
                    }

                    const math::real pdf_j = m_bxdfs[j]->pdf(surfaceInteraction, i_local, o_local);

                    if (pdf_j == 0) continue;
                    else *pdf += pdf_j;

                    math::Vector f_i = m_bxdfs[j]->f(surfaceInteraction, i_local, o_local, stackAllocator);
                    if (cosineWeight) {
                        f_i = math::mul(f_i, math::abs(math::expandZ(o_local)));
                    }

                    f = math::add(f, f_i);
                }
            }
        }

        *pdf /= m_bxdfCount;
    }

    return f;
}

manta::math::Vector manta::BSDF::f(
    const IntersectionPoint *surfaceInteraction,
    const math::Vector &i,
    const math::Vector &o,
    bool cosineWeight) const
{
    math::Vector f = math::constants::Zero;

    const int bxdfCount = getBxdfCount();
    for (int j = 0; j < bxdfCount; ++j) {
        math::Vector basis_u, basis_v, basis_w;
        m_bxdfs[j]->generateBasisVectors(i, surfaceInteraction, &basis_u, &basis_v, &basis_w);

        const math::Vector i_local = m_bxdfs[j]->transform(i, basis_u, basis_v, basis_w);
        const math::Vector o_local = m_bxdfs[j]->transform(o, basis_u, basis_v, basis_w);

        if (math::getZ(i_local) <= 0 || math::getZ(o_local) <= 0) {
            continue;
        }

        math::Vector f_i = m_bxdfs[j]->f(surfaceInteraction, i_local, o_local, nullptr);
        if (cosineWeight) {
            f_i = math::mul(f_i, math::expandZ(o_local));
        }

        f = math::add(f, f_i);
    }

    return f;
}

manta::math::real manta::BSDF::pdf(
    const IntersectionPoint *surfaceInteraction,
    const math::Vector &i,
    const math::Vector &o) const
{
    math::real pdf = 0;
    for (int j = 0; j < m_bxdfCount; j++) {
        math::Vector basis_u, basis_v, basis_w;
        m_bxdfs[j]->generateBasisVectors(i, surfaceInteraction, &basis_u, &basis_v, &basis_w);

        const math::Vector i_local = m_bxdfs[j]->transform(i, basis_u, basis_v, basis_w);
        const math::Vector o_local = m_bxdfs[j]->transform(o, basis_u, basis_v, basis_w);

        if (math::getZ(i_local) <= 0 || math::getZ(o_local) <= 0) {
            continue;
        }

        pdf += m_bxdfs[j]->pdf(surfaceInteraction, i_local, o_local);
    }

    pdf /= m_bxdfCount;

    return pdf;
}

void manta::BSDF::_evaluate() {
    setOutput(this);
}
