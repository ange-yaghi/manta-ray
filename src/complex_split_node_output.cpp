#include "../include/complex_split_node_output.h"

#include "../include/complex_node_output.h"
#include "../include/complex_math.h"

manta::ComplexSplitNodeOutput::ComplexSplitNodeOutput() {
    m_input = nullptr;
    m_valueIndex = 0;
}

manta::ComplexSplitNodeOutput::~ComplexSplitNodeOutput() {
    /* void */
}

void manta::ComplexSplitNodeOutput::sample(
    const IntersectionPoint *surfaceInteraction,
    void *target_) const
{
    assert(m_valueIndex != -1);

    math::Vector *target = reinterpret_cast<math::Vector *>(target_);
    math::Complex value;

    ComplexNodeOutput *input = static_cast<ComplexNodeOutput *>(m_input);
    input->sample(surfaceInteraction, (void *)&value);

    *target = math::loadScalar((math::real)value.values[m_valueIndex]);
}

void manta::ComplexSplitNodeOutput::discreteSample2d(int x, int y, void *target_) const {
    math::Vector *target = reinterpret_cast<math::Vector *>(target_);
    math::Complex value;

    ComplexNodeOutput *input = static_cast<ComplexNodeOutput *>(m_input);
    input->discreteSample2d(x, y, (void *)&value);

    *target = math::loadScalar((math::real)value.values[m_valueIndex]);
}

void manta::ComplexSplitNodeOutput::fullOutput(const void **target) const {
    *target = nullptr;
}

void manta::ComplexSplitNodeOutput::registerInputs() {
    registerInput(&m_input);
}

void manta::ComplexSplitNodeOutput::_evaluateDimensions() {
    ComplexNodeOutput *input = static_cast<ComplexNodeOutput *>(m_input);
    input->evaluateDimensions();

    const int dimensions = input->getDimensions();
    setDimensions(dimensions);

    for (int i = 0; i < dimensions; i++) {
        const int size = (input->getSize(i) > 0)
            ? input->getSize(i)
            : 0;
        setDimensionSize(i, size);
    }
}
