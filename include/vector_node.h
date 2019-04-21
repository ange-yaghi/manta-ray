#ifndef VECTOR_NODE_H
#define VECTOR_NODE_H

#include <node.h>

#include <manta_math.h>

namespace manta {

	struct IntersectionPoint;

	class VectorNode : public Node {
	public:
		VectorNode();
		~VectorNode();

		virtual math::Vector sample(const IntersectionPoint *surfaceInteraction) const = 0;
	};

} /* namespace manta */

#endif /* VECTOR_NODE_H */
