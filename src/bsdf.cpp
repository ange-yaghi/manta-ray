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

manta::math::Vector manta::BSDF::sampleF(const IntersectionPoint *surfaceInteraction,
    const math::Vector2 &u, const math::Vector &i, math::Vector *o, math::real *pdf,
    StackAllocator *stackAllocator) const 
{
    int bxdf = rand() % m_bxdfCount;
    math::Vector f = m_bxdfs[bxdf]->sampleF(surfaceInteraction, u, i, o, pdf, stackAllocator);

    if (*pdf == (math::real)0.0) {
        return math::constants::Zero;
    }

    if (m_bxdfCount > 1) {
        for (int j = 0; j < m_bxdfCount; j++) {
            if (j != bxdf) {
                *pdf += m_bxdfs[j]->pdf(surfaceInteraction, i, *o);
            }
        }

        *pdf /= m_bxdfCount;

        f = math::constants::Zero;
        for (int j = 0; j < m_bxdfCount; j++) {
            f = math::add(f, m_bxdfs[j]->f(surfaceInteraction, i, *o, stackAllocator));
        }
    }

    return f;
}

manta::math::Vector manta::BSDF::f(
    const IntersectionPoint *surfaceInteraction, const math::Vector &i, const math::Vector &o) const
{
    const math::Vector local_i = surfaceInteraction->worldToLocal(i);
    const math::Vector local_o = surfaceInteraction->worldToLocal(o);

    const int bxdfCount = getBxdfCount();
    for (int i = 0; i < bxdfCount; i++) {
        m_bxdfs[i]->f(surfaceInteraction, local_i, local_o, nullptr);
    }

    return math::constants::Zero;
}

void manta::BSDF::_evaluate() {
    setOutput(this);
}
