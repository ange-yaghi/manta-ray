#include "../include/vector_split_node_output.h"

#include <assert.h>

manta::VectorSplitNodeOutput::VectorSplitNodeOutput() : VectorNodeOutput(true) {
    m_input = nullptr;
    m_valueIndex = -1;
}

manta::VectorSplitNodeOutput::~VectorSplitNodeOutput() {
    /* void */
}

void manta::VectorSplitNodeOutput::sample(const IntersectionPoint *surfaceInteraction, void *_target) const {
    assert(m_valueIndex != -1);

    math::Vector *target = reinterpret_cast<math::Vector *>(_target);
    math::Vector value;

    VectorNodeOutput *input = static_cast<VectorNodeOutput *>(m_input);
    input->sample(surfaceInteraction, (void *)&value);

    *target = math::loadScalar(math::get(value, m_valueIndex));
}

void manta::VectorSplitNodeOutput::discreteSample2d(int x, int y, void *_target) const {
    math::Vector *target = reinterpret_cast<math::Vector *>(_target);
    math::Vector value;

    VectorNodeOutput *input = static_cast<VectorNodeOutput *>(m_input);
    input->discreteSample2d(x, y, (void *)&value);

    *target = math::loadScalar(math::get(value, m_valueIndex));
}

void manta::VectorSplitNodeOutput::fullOutput(const void **_target) const {
    // TODO
    *_target = nullptr;
}

void manta::VectorSplitNodeOutput::registerInputs() {
    registerInput(&m_input);
}

void manta::VectorSplitNodeOutput::_evaluateDimensions() {
    VectorNodeOutput *input = static_cast<VectorNodeOutput *>(m_input);

    input->evaluateDimensions();

    const int dimensions = input->getDimensions();
    setDimensions(dimensions);

    for (int i = 0; i < dimensions; i++) {
        int size = 0;
        if (input->getSize(i) > size) size = input->getSize(i);

        setDimensionSize(i, size);
    }
}
