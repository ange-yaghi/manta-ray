#include "../include/constructed_complex_node_output.h"

#include "../include/complex_math.h"
#include "../include/vector_node_output.h"

manta::ConstructedComplexNodeOutput::ConstructedComplexNodeOutput() {
    m_r = nullptr;
    m_i = nullptr;
}

manta::ConstructedComplexNodeOutput::~ConstructedComplexNodeOutput() {
    /* void */
}

void manta::ConstructedComplexNodeOutput::sample(
    const IntersectionPoint *surfaceInteraction,
    void *_target) const
{
    math::Complex *target = reinterpret_cast<math::Complex *>(_target);
    math::Vector v_r, v_i;

    VectorNodeOutput *r = static_cast<VectorNodeOutput *>(m_r);
    VectorNodeOutput *i = static_cast<VectorNodeOutput *>(m_i);

    r->sample(surfaceInteraction, (void *)&v_r);
    i->sample(surfaceInteraction, (void *)&v_i);

    *target = math::Complex(
        (math::real_d)math::getScalar(v_r),
        (math::real_d)math::getScalar(v_i));
}

void manta::ConstructedComplexNodeOutput::discreteSample2d(int x, int y, void *_target) const {
    math::Complex *target = reinterpret_cast<math::Complex *>(_target);
    math::Vector v_r, v_i;

    static_cast<VectorNodeOutput *>(m_r)->discreteSample2d(x, y, (void *)&v_r);
    static_cast<VectorNodeOutput *>(m_i)->discreteSample2d(x, y, (void *)&v_i);

    *target = math::Complex(
        math::getScalar(v_r),
        math::getScalar(v_i)
    );
}

void manta::ConstructedComplexNodeOutput::fullOutput(const void **_target) const {
    // TODO
    *_target = nullptr;
}

void manta::ConstructedComplexNodeOutput::registerInputs() {
    registerInput(&m_r);
    registerInput(&m_i);
}

void manta::ConstructedComplexNodeOutput::_evaluateDimensions() {
    VectorNodeOutput *r = static_cast<VectorNodeOutput *>(m_r);
    VectorNodeOutput *i = static_cast<VectorNodeOutput *>(m_i);

    r->evaluateDimensions();
    i->evaluateDimensions();

    int dimensions = 0;
    if (r->getDimensions() > dimensions) dimensions = r->getDimensions();
    if (i->getDimensions() > dimensions) dimensions = i->getDimensions();

    setDimensions(dimensions);

    for (int j = 0; j < dimensions; j++) {
        int size = 0;
        if (r->getDimensions() > j && r->getSize(j) > size) size = r->getSize(j);
        if (i->getDimensions() > j && i->getSize(j) > size) size = i->getSize(j);

        setDimensionSize(j, size);
    }
}
