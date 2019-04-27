#include <single_vector_node_output.h>

void manta::SingleVectorNodeOutput::sample(const IntersectionPoint *surfaceInteraction, void *_target) const {
	math::Vector *target = reinterpret_cast<math::Vector *>(_target);

	*target = m_value;
}

void manta::SingleVectorNodeOutput::discreteSample2D(int x, int y, void *_target) const {
	math::Vector *target = reinterpret_cast<math::Vector *>(_target);

	*target = m_value;
}

void manta::SingleVectorNodeOutput::fullOutput(const void **_target) const {
	math::Vector *target = reinterpret_cast<math::Vector *>(_target);

	*target = m_value;
}
