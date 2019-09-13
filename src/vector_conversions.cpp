#include "../include/vector_conversions.h"

#include "../include/cached_vector_node.h"

manta::FloatToVectorConversionOutput::FloatToVectorConversionOutput() {
    m_input = nullptr;
}

manta::FloatToVectorConversionOutput::~FloatToVectorConversionOutput() {
    /* void */
}

void manta::FloatToVectorConversionOutput::sample(
    const IntersectionPoint *surfaceInteraction, void *_target) const 
{
    (void)surfaceInteraction;

    piranha::native_float value;
    m_input->fullCompute((void *)&value);

    math::Vector *target = reinterpret_cast<math::Vector *>(_target);

    *target = math::loadScalar((math::real)value);
}

void manta::FloatToVectorConversionOutput::discreteSample2d(int x, int y, void *target) const {
    (void)x;
    (void)y;

    sample(nullptr, target);
}

void manta::FloatToVectorConversionOutput::registerInputs() {
    registerInput(&m_input);
}

manta::IntToVectorConversionOutput::IntToVectorConversionOutput() {
    m_input = nullptr;
}

manta::IntToVectorConversionOutput::~IntToVectorConversionOutput() {
    /* void */
}

void manta::IntToVectorConversionOutput::sample(
    const IntersectionPoint *surfaceInteraction, void *_target) const 
{
    (void)surfaceInteraction;

    piranha::native_int value;
    m_input->fullCompute((void *)&value);

    math::Vector *target = reinterpret_cast<math::Vector *>(_target);

    *target = math::loadScalar((math::real)value);
}

void manta::IntToVectorConversionOutput::discreteSample2d(int x, int y, void *target) const {
    (void)x;
    (void)y;

    sample(nullptr, target);
}

void manta::IntToVectorConversionOutput::registerInputs() {
    registerInput(&m_input);
}

piranha::Node *manta::IntToVectorConversionOutput::_optimize() {
    return nullptr;
}
