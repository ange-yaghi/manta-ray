#include <vector_split_node_output.h>

#include <assert.h>

manta::VectorSplitNodeOutput::VectorSplitNodeOutput() : VectorNodeOutput(true) {
	m_valueIndex = -1;
}

manta::VectorSplitNodeOutput::~VectorSplitNodeOutput() {
	/* void */
}

void manta::VectorSplitNodeOutput::sample(const IntersectionPoint *surfaceInteraction, void *_target) const {
	(void)surfaceInteraction;

	assert(m_valueIndex != -1);

	math::Vector *target = reinterpret_cast<math::Vector *>(_target);
	math::Vector value;

	m_input->sample(surfaceInteraction, (void *)&value);

	*target = math::loadScalar(math::get(value, m_valueIndex));
}

void manta::VectorSplitNodeOutput::discreteSample2D(int x, int y, void *target) const {
	(void)x;
	(void)y;

	sample(nullptr, target);
}

void manta::VectorSplitNodeOutput::fullOutput(const void **_target) const {
	// TODO
	*_target = nullptr;
}
