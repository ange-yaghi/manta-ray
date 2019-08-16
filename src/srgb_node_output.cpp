#include "../include/srgb_node_output.h"

#include "../include/rgb_space.h"
#include "../include/color.h"


manta::SrgbNodeOutput::SrgbNodeOutput() {
    /* void */
}

manta::SrgbNodeOutput::~SrgbNodeOutput() {
    /* void */
}

void manta::SrgbNodeOutput::sample(const IntersectionPoint *surfaceInteraction, void *_target) const {
    (void)surfaceInteraction;

    math::Vector *target = reinterpret_cast<math::Vector *>(_target);
    math::Vector v_r, v_g, v_b, v_a;

    VectorNodeOutput *r = static_cast<VectorNodeOutput *>(m_r);
    VectorNodeOutput *g = static_cast<VectorNodeOutput *>(m_g);
    VectorNodeOutput *b = static_cast<VectorNodeOutput *>(m_b);
    VectorNodeOutput *a = static_cast<VectorNodeOutput *>(m_a);

    r->sample(surfaceInteraction, (void *)&v_r);
    g->sample(surfaceInteraction, (void *)&v_g);
    b->sample(surfaceInteraction, (void *)&v_b);
    a->sample(surfaceInteraction, (void *)&v_a);

    math::real_d r_d = math::getScalar(v_r);
    math::real_d g_d = math::getScalar(v_g);
    math::real_d b_d = math::getScalar(v_b);
    math::real_d a_d = math::getScalar(v_a);

    r_d = RgbSpace::srgb.inverseGammaSrgb(r_d);
    g_d = RgbSpace::srgb.inverseGammaSrgb(g_d);
    b_d = RgbSpace::srgb.inverseGammaSrgb(b_d);
    a_d = RgbSpace::srgb.inverseGammaSrgb(a_d);

    *target = math::loadVector(
        (math::real)r_d,
        (math::real)g_d,
        (math::real)b_d,
        (math::real)a_d
    );
}

void manta::SrgbNodeOutput::discreteSample2D(int x, int y, void *target) const {
    (void)x;
    (void)y;

    sample(nullptr, target);
}

void manta::SrgbNodeOutput::fullOutput(const void **_target) const {
    // TODO
    *_target = nullptr;
}

void manta::SrgbNodeOutput::registerInputs() {
    registerInput(&m_r);
    registerInput(&m_g);
    registerInput(&m_b);
    registerInput(&m_a);
}
