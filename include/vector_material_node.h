#ifndef VECTOR_MATERIAL_NODE_H
#define VECTOR_MATERIAL_NODE_H

#include <material_node.h>

#include <manta_math.h>

namespace manta {

	struct IntersectionPoint;

	class VectorMaterialNode : public MaterialNode {
	public:
		VectorMaterialNode();
		~VectorMaterialNode();

		virtual math::Vector sample(const IntersectionPoint *surfaceInteraction) const = 0;
	};

} /* namespace manta */

#endif /* VECTOR_MATERIAL_NODE_H */
