#include "..\include\vector_conversions.h"

manta::FloatToVectorConversionOutput::FloatToVectorConversionOutput() {
    /* void */
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
