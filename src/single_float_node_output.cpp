#include <single_float_node_output.h>

manta::SingleFloatNodeOutput::SingleFloatNodeOutput() {
	setDimensions(1);
	setDimensionSize(0, 1);

	m_value = math::constants::Zero;
}

manta::SingleFloatNodeOutput::~SingleFloatNodeOutput() {
	/* void */
}

void manta::SingleFloatNodeOutput::sample(const IntersectionPoint *surfaceInteraction, void *_target) const {
	math::Vector *target = reinterpret_cast<math::Vector *>(_target);

	*target = m_value;
}

void manta::SingleFloatNodeOutput::discreteSample2D(int x, int y, void *_target) const {
	math::Vector *target = reinterpret_cast<math::Vector *>(_target);

	*target = m_value;
}

void manta::SingleFloatNodeOutput::fullOutput(const void **_target) const {
	math::Vector *target = reinterpret_cast<math::Vector *>(_target);

	*target = m_value;
}
