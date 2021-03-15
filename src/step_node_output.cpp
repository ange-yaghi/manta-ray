#include "../include/step_node_output.h"

void manta::StepNodeOutput::sample(const IntersectionPoint *surfaceInteraction, void *_target) const {
    VectorNodeOutput *input = static_cast<VectorNodeOutput *>(m_input);
    VectorNodeOutput *dc = static_cast<VectorNodeOutput *>(m_dc);
    VectorNodeOutput *foot = static_cast<VectorNodeOutput *>(m_foot);
    VectorNodeOutput *step = static_cast<VectorNodeOutput *>(m_step);
    
    math::Vector *target = reinterpret_cast<math::Vector *>(_target);
    math::Vector
        v_input = math::constants::Zero,
        v_dc = m_defaultDc,
        v_foot = m_defaultFoot,
        v_step = m_defaultStep;

    if (input != nullptr) {
        input->sample(surfaceInteraction, (void *)&v_input);
    }

    if (dc != nullptr) {
        dc->sample(surfaceInteraction, (void *)&v_dc);
    }

    if (foot != nullptr) {
        foot->sample(surfaceInteraction, (void *)&v_foot);
    }

    if (step != nullptr) {
        step->sample(surfaceInteraction, (void *)&v_step);
    }

    *target = stepFunction(v_input, v_dc, v_foot, v_step);
}

void manta::StepNodeOutput::discreteSample2d(int x, int y, void *_target) const {
    VectorNodeOutput *input = static_cast<VectorNodeOutput *>(m_input);
    VectorNodeOutput *dc = static_cast<VectorNodeOutput *>(m_dc);
    VectorNodeOutput *foot = static_cast<VectorNodeOutput *>(m_foot);
    VectorNodeOutput *step = static_cast<VectorNodeOutput *>(m_step);

    math::Vector *target = reinterpret_cast<math::Vector *>(_target);
    math::Vector
        v_input = math::constants::Zero,
        v_dc = m_defaultDc,
        v_foot = m_defaultFoot,
        v_step = m_defaultStep;

    if (input != nullptr) {
        input->discreteSample2d(x, y, (void *)&v_input);
    }

    if (dc != nullptr) {
        dc->discreteSample2d(x, y, (void *)&v_dc);
    }

    if (foot != nullptr) {
        foot->discreteSample2d(x, y, (void *)&v_foot);
    }

    if (step != nullptr) {
        step->discreteSample2d(x, y, (void *)&v_step);
    }

    *target = stepFunction(v_input, v_dc, v_foot, v_step);
}

manta::math::Vector manta::StepNodeOutput::stepFunction(const math::Vector &input, const math::Vector &dc, const math::Vector &foot, const math::Vector &step) {
    math::Vector s = math::sub(input, foot);
    s = math::gt(s, math::constants::Zero);

    s = math::mul(s, math::sub(step, dc));
    s = math::add(s, dc);

    return s;
}

void manta::StepNodeOutput::registerInputs() {
    registerInput(&m_input);
}

void manta::StepNodeOutput::_evaluateDimensions() {
    VectorNodeOutput *input = static_cast<VectorNodeOutput *>(m_input);
    VectorNodeOutput *dc = static_cast<VectorNodeOutput *>(m_dc);
    VectorNodeOutput *foot = static_cast<VectorNodeOutput *>(m_foot);
    VectorNodeOutput *step = static_cast<VectorNodeOutput *>(m_step);

    input->evaluateDimensions();
    dc->evaluateDimensions();
    foot->evaluateDimensions();
    step->evaluateDimensions();

    int dimensions = 0;
    if (input->getDimensions() > dimensions) dimensions = input->getDimensions();
    if (dc->getDimensions() > dimensions) dimensions = dc->getDimensions();
    if (foot->getDimensions() > dimensions) dimensions = foot->getDimensions();
    if (step->getDimensions() > dimensions) dimensions = step->getDimensions();

    setDimensions(dimensions);

    for (int i = 0; i < dimensions; i++) {
        int size = 0;
        if (input->getDimensions() > i && input->getSize(i) > size) size = input->getSize(i);
        if (dc->getDimensions() > i && dc->getSize(i) > size) size = dc->getSize(i);
        if (foot->getDimensions() > i && foot->getSize(i) > size) size = foot->getSize(i);
        if (step->getDimensions() > i && step->getSize(i) > size) size = step->getSize(i);

        setDimensionSize(i, size);
    }
}
