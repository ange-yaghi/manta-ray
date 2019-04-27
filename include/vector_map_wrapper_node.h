#ifndef VECTOR_MAP_WRAPPER_NODE_H
#define VECTOR_MAP_WRAPPER_NODE_H

#include <node.h>

#include <vector_map_2d_node_output.h>

namespace manta {

	class VectorMapWrapperNode : public Node {
	public:
		VectorMapWrapperNode(const VectorMap2D *map) {
			m_map = map;
		}

		virtual ~VectorMapWrapperNode() {
			/* void */
		}

		const VectorMap2DNodeOutput *getMainOutput() const { return &m_output; }

	protected:
		virtual void _evaluate() {
			m_output.setMap(m_map);
		}

	protected:
		virtual void registerOutputs() {
			registerOutput(&m_output, "Output");
		}

		VectorMap2DNodeOutput m_output;

		const VectorMap2D *m_map;
	};

} /* namespace manta */

#endif /* VECTOR_MAP_WRAPPER_NODE_H */
