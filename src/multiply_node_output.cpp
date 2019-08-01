#include "../include/multiply_node_output.h"

#include <assert.h>

void manta::MultiplyNodeOutput::sample(const IntersectionPoint *surfaceInteraction, void *_target) const {
    math::Vector *target = reinterpret_cast<math::Vector *>(_target);
    math::Vector a, b;
    a = m_defaultA;
    b = m_defaultB;

    VectorNodeOutput *inputA = static_cast<VectorNodeOutput *>(m_inputA);
    VectorNodeOutput *inputB = static_cast<VectorNodeOutput *>(m_inputB);

    if (inputA != nullptr) {
        inputA->sample(surfaceInteraction, (void *)&a);
    }
    if (inputB != nullptr) {
        inputB->sample(surfaceInteraction, (void *)&b);
    }

    *target = math::mul(a, b);
}

void manta::MultiplyNodeOutput::discreteSample2D(int x, int y, void *_target) const {
    math::Vector *target = reinterpret_cast<math::Vector *>(_target);
    math::Vector a, b;
    a = m_defaultA;
    b = m_defaultB;

    VectorNodeOutput *inputA = static_cast<VectorNodeOutput *>(m_inputA);
    VectorNodeOutput *inputB = static_cast<VectorNodeOutput *>(m_inputB);

    if (inputA != nullptr) {
        inputA->discreteSample2d(x, y, (void *)&a);
    }
    if (inputB != nullptr) {
        inputB->discreteSample2d(x, y, (void *)&b);
    }

    *target = math::mul(a, b);
}

void manta::MultiplyNodeOutput::getDataReference(const void **_target) const {
    *_target = nullptr;
}

void manta::MultiplyNodeOutput::_evaluateDimensions() {
    if (m_inputA == nullptr && m_inputB == nullptr) {
        setDimensions(1);
        setDimensionSize(0, 1);
        return;
    }

    VectorNodeOutput *inputA = static_cast<VectorNodeOutput *>(m_inputA);
    VectorNodeOutput *inputB = static_cast<VectorNodeOutput *>(m_inputB);

    if (m_inputA != nullptr) assert(inputA->areDimensionsEvaluated());
    if (m_inputB != nullptr) assert(inputB->areDimensionsEvaluated());

    int dimensions = 0;
    if (m_inputA != nullptr && inputA->getDimensions() > dimensions) dimensions = inputA->getDimensions();
    if (m_inputB != nullptr && inputB->getDimensions() > dimensions) dimensions = inputB->getDimensions();

    setDimensions(dimensions);

    for (int i = 0; i < dimensions; i++) {
        int size = 0;
        if (m_inputA != nullptr && inputA->getSize(i) > size) size = inputA->getSize(i);
        if (m_inputB != nullptr && inputB->getSize(i) > size) size = inputB->getSize(i);

        setDimensionSize(i, size);
    }
}
