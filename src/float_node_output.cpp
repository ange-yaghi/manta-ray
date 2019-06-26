#include <float_node_output.h>

#include <vector_map_2d.h>

const manta::NodeType manta::FloatNodeOutput::FloatType("FloatNodeType");

void manta::FloatNodeOutput::fullCompute(void *_target) const {
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
