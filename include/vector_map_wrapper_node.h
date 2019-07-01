#ifndef VECTOR_MAP_WRAPPER_NODE_H
#define VECTOR_MAP_WRAPPER_NODE_H

#include <node.h>

#include <vector_map_2d_node_output.h>

namespace manta {

	class VectorMapWrapperNode : public Node {
	public:
		VectorMapWrapperNode(const VectorMap2D *map) {
			m_output.setMap(map);
		}

		virtual ~VectorMapWrapperNode() {
			/* void */
		}

		VectorMap2DNodeOutput *getMainOutput() { return &m_output; }

	protected:
		virtual void _evaluate() {
			/* void */
		}

	protected:
		virtual void registerOutputs() {
			registerOutput(&m_output, "Output");
		}

		VectorMap2DNodeOutput m_output;
	};

} /* namespace manta */

#endif /* VECTOR_MAP_WRAPPER_NODE_H */
