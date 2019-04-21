#ifndef VECTOR_MAP_WRAPPER_NODE_H
#define VECTOR_MAP_WRAPPER_NODE_H

#include <node.h>

#include <map_2d_node.h>

namespace manta {

	class VectorMapWrapperNode : public VectorMap2DNode {
	public:
		VectorMapWrapperNode(const VectorMap2D *map) {
			m_map = map;
		}

		virtual ~VectorMapWrapperNode() {
			/* void */
		}
	};

} /* namespace manta */

#endif /* VECTOR_MAP_WRAPPER_NODE_H */
