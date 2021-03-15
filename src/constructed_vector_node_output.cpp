#include "../include/constructed_vector_node_output.h"

manta::ConstructedVectorNodeOutput::ConstructedVectorNodeOutput() {
    /* void */
}

manta::ConstructedVectorNodeOutput::~ConstructedVectorNodeOutput() {
    /* void */
}

void manta::ConstructedVectorNodeOutput::sample(const IntersectionPoint *surfaceInteraction, void *_target) const {
    (void)surfaceInteraction;

    math::Vector *target = reinterpret_cast<math::Vector *>(_target);
    math::Vector v_x, v_y, v_z, v_w;

    VectorNodeOutput *x = static_cast<VectorNodeOutput *>(m_x);
    VectorNodeOutput *y = static_cast<VectorNodeOutput *>(m_y);
    VectorNodeOutput *z = static_cast<VectorNodeOutput *>(m_z);
    VectorNodeOutput *w = static_cast<VectorNodeOutput *>(m_w);

    x->sample(surfaceInteraction, (void *)&v_x);
    y->sample(surfaceInteraction, (void *)&v_y);
    z->sample(surfaceInteraction, (void *)&v_z);
    w->sample(surfaceInteraction, (void *)&v_w);

    *target = math::loadVector(
        math::getScalar(v_x),
        math::getScalar(v_y),
        math::getScalar(v_z),
        math::getScalar(v_w)
    );
}

void manta::ConstructedVectorNodeOutput::discreteSample2d(int x, int y, void *_target) const {
    math::Vector *target = reinterpret_cast<math::Vector *>(_target);
    math::Vector v_x, v_y, v_z, v_w;

    static_cast<VectorNodeOutput *>(m_x)->discreteSample2d(x, y, (void *)&v_x);
    static_cast<VectorNodeOutput *>(m_y)->discreteSample2d(x, y, (void *)&v_y);
    static_cast<VectorNodeOutput *>(m_z)->discreteSample2d(x, y, (void *)&v_z);
    static_cast<VectorNodeOutput *>(m_w)->discreteSample2d(x, y, (void *)&v_w);

    *target = math::loadVector(
        math::getScalar(v_x),
        math::getScalar(v_y),
        math::getScalar(v_z),
        math::getScalar(v_w)
    );
}

void manta::ConstructedVectorNodeOutput::fullOutput(const void **_target) const {
    // TODO
    *_target = nullptr;
}

void manta::ConstructedVectorNodeOutput::registerInputs() {
    registerInput(&m_x);
    registerInput(&m_y);
    registerInput(&m_z);
    registerInput(&m_w);
}

void manta::ConstructedVectorNodeOutput::_evaluateDimensions() {
    VectorNodeOutput *x = static_cast<VectorNodeOutput *>(m_x);
    VectorNodeOutput *y = static_cast<VectorNodeOutput *>(m_y);
    VectorNodeOutput *z = static_cast<VectorNodeOutput *>(m_z);
    VectorNodeOutput *w = static_cast<VectorNodeOutput *>(m_w);

    x->evaluateDimensions();
    y->evaluateDimensions();
    z->evaluateDimensions();
    w->evaluateDimensions();

    int dimensions = 0;
    if (x->getDimensions() > dimensions) dimensions = x->getDimensions();
    if (y->getDimensions() > dimensions) dimensions = y->getDimensions();
    if (z->getDimensions() > dimensions) dimensions = z->getDimensions();
    if (w->getDimensions() > dimensions) dimensions = w->getDimensions();

    setDimensions(dimensions);

    for (int i = 0; i < dimensions; i++) {
        int size = 0;
        if (x->getDimensions() > i && x->getSize(i) > size) size = x->getSize(i);
        if (y->getDimensions() > i && y->getSize(i) > size) size = y->getSize(i);
        if (z->getDimensions() > i && z->getSize(i) > size) size = z->getSize(i);
        if (w->getDimensions() > i && w->getSize(i) > size) size = w->getSize(i);

        setDimensionSize(i, size);
    }
}
