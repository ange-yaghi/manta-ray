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
	math::Vector v_x, v_y, v_z, v_w;

    VectorNodeOutput *x = static_cast<VectorNodeOutput *>(m_x);
    VectorNodeOutput *y = static_cast<VectorNodeOutput *>(m_y);
    VectorNodeOutput *z = static_cast<VectorNodeOutput *>(m_z);
    VectorNodeOutput *w = static_cast<VectorNodeOutput *>(m_w);

	x->sample(surfaceInteraction, (void *)&v_x);
	y->sample(surfaceInteraction, (void *)&v_y);
	z->sample(surfaceInteraction, (void *)&v_z);
	w->sample(surfaceInteraction, (void *)&v_w);

	*target = math::loadVector(
		math::getScalar(v_x),
		math::getScalar(v_y),
		math::getScalar(v_z),
		math::getScalar(v_w)
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

void manta::ConstructedVectorNodeOutput::registerInputs() {
	registerInput(&m_x);
	registerInput(&m_y);
	registerInput(&m_z);
	registerInput(&m_w);
}
