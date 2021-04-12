#include "../include/bump_node_output.h"

manta::BumpNodeOutput::BumpNodeOutput() {
    m_displacement = nullptr;
    m_normal = nullptr;
}

manta::BumpNodeOutput::~BumpNodeOutput() {
    /* void */
}

void manta::BumpNodeOutput::sample(const IntersectionPoint *surfaceInteraction, void *target) const {
    const math::real du = 0.001f;
    const math::real dv = 0.001f;

    IntersectionPoint main = *surfaceInteraction;

    VectorNodeOutput *displacement = static_cast<VectorNodeOutput *>(m_displacement);
    VectorNodeOutput *normal = static_cast<VectorNodeOutput *>(m_normal);

    math::Vector disp0, disp1, disp2;
    math::Vector norm0, norm1, norm2;

    normal->sample(surfaceInteraction, &norm0);

    main.m_vertexNormal = norm0;
    main.calculatePartialDerivatives();

    IntersectionPoint offsetPoint0 = main;
    IntersectionPoint offsetPoint1 = main;

    offsetPoint0.offset(du, (math::real)0.0);
    offsetPoint1.offset((math::real)0.0, dv);

    displacement->sample(&main, &disp0);
    displacement->sample(&offsetPoint0, &disp1);
    displacement->sample(&offsetPoint1, &disp2);
    
    normal->sample(&offsetPoint0, &norm1);
    normal->sample(&offsetPoint1, &norm2);

    const math::Vector p0 =
        math::add(main.m_position, math::mul(disp0, norm0));
    const math::Vector p1 =
        math::add(offsetPoint0.m_position, math::mul(disp1, norm1));
    const math::Vector p2 =
        math::add(offsetPoint1.m_position, math::mul(disp2, norm2));

    const math::Vector p1p0 = math::sub(p1, p0);
    const math::Vector p2p0 = math::sub(p2, p0);

    const math::Vector bumpNormal = math::normalize(math::cross(p1p0, p2p0));

    math::Vector *result = reinterpret_cast<math::Vector *>(target);
    *result = bumpNormal;
}

void manta::BumpNodeOutput::discreteSample2d(int x, int y, void *target) const {
    /* void */
}

void manta::BumpNodeOutput::fullOutput(const void **target) const {
    /* void */
}

void manta::BumpNodeOutput::registerInputs() {
    registerInput(&m_displacement);
    registerInput(&m_normal);
}
