#include <constructed_vector_node_output.h>

manta::ConstructedVectorNodeOutput::ConstructedVectorNodeOutput() {
	/* void */
}

manta::ConstructedVectorNodeOutput::~ConstructedVectorNodeOutput() {
	/* void */
}

void manta::ConstructedVectorNodeOutput::sample(const IntersectionPoint *surfaceInteraction, void *_target) const {
	(void)surfaceInteraction;

	math::Vector *target = reinterpret_cast<math::Vector *>(_target);
	math::Vector x, y, z, w;

	m_x->sample(surfaceInteraction, (void *)&x);
	m_y->sample(surfaceInteraction, (void *)&y);
	m_z->sample(surfaceInteraction, (void *)&z);
	m_w->sample(surfaceInteraction, (void *)&w);

	*target = math::loadVector(
		math::getScalar(x),
		math::getScalar(y),
		math::getScalar(z),
		math::getScalar(w)
	);
}

void manta::ConstructedVectorNodeOutput::discreteSample2D(int x, int y, void *target) const {
	(void)x;
	(void)y;

	sample(nullptr, target);
}

void manta::ConstructedVectorNodeOutput::fullOutput(const void **_target) const {
	// TODO
	*_target = nullptr;
}
