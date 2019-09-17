#include "../include/bsdf.h"

#include "../include/bxdf.h"
#include "../include/intersection_point.h"

manta::BSDF::BSDF() : ObjectReferenceNode<BSDF>() {
    m_output.setReference(this);

    m_bxdfCount = 0;
}

manta::BSDF::BSDF(BXDF *bxdf) {
    m_output.setReference(this);

    addBxdf(bxdf);
}

manta::BSDF::~BSDF() {
    /* void */
}

manta::math::Vector manta::BSDF::sampleF(const IntersectionPoint *surfaceInteraction,
    const math::Vector &i, math::Vector *o, math::real *pdf,
    StackAllocator *stackAllocator) const 
{
    return m_bxdfs[0]->sampleF(surfaceInteraction, i, o, pdf, stackAllocator);
}

manta::math::Vector manta::BSDF::f(
    const IntersectionPoint *surfaceInteraction, const math::Vector &i, const math::Vector &o) const
{
    math::Vector local_i = surfaceInteraction->worldToLocal(i);
    math::Vector local_o = surfaceInteraction->worldToLocal(o);

    bool isReflection = surfaceInteraction->isReflection(i, o);

    int bxdfCount = getBxdfCount();
    for (int i = 0; i < bxdfCount; i++) {
        m_bxdfs[i]->f(surfaceInteraction, local_i, local_o, nullptr);
    }

    return math::constants::Zero;
}
