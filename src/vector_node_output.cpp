#include <vector_node_output.h>

#include <vector_map_2d.h>
#include <vector_split_node.h>
#include <standard_allocator.h>

const manta::NodeType manta::VectorNodeOutput::VectorType("VectorNodeType");

manta::VectorNodeOutput::VectorNodeOutput(bool scalar) : NodeOutput(&VectorType) {
	m_scalar = scalar;
}

manta::VectorNodeOutput::~VectorNodeOutput() {
	/* void */
}

void manta::VectorNodeOutput::fullCompute(void *_target) const {
	VectorMap2D *target = reinterpret_cast<VectorMap2D *>(_target);

	int width, height;

	int dimensions = getDimensions();
	if (dimensions == 0) {
		return;
	}
	else if (dimensions == 1) {
		width = getSize(0);
		height = 1;
	}
	else if (dimensions == 2) {
		width = getSize(0);
		height = getSize(1);
	}
	else {
		// Dimensions higher than 2 not currently supported
		return;
	}

	target->initialize(width, height);

	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			math::Vector v;
			discreteSample2D(i, j, &v);
			target->set(v, i, j);
		}
	}
}

manta::Node *manta::VectorNodeOutput::generateInterface() {
	if (!m_scalar) {
		VectorSplitNode *vectorInterface = 
			StandardAllocator::Global()->allocate<VectorSplitNode>(16);
		vectorInterface->initialize();
		vectorInterface->connectInput(this, "__in");

		return vectorInterface;
	}
	else return nullptr;
}
