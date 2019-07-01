#ifndef VECTOR_SPLIT_NODE_H
#define VECTOR_SPLIT_NODE_H

#include <node.h>

#include <vector_split_node_output.h>

namespace manta {

	class VectorSplitNode : public Node {
	public:
		VectorSplitNode();
		virtual ~VectorSplitNode();

	protected:
		virtual void _initialize();
		virtual void _evaluate();
		virtual void _destroy();

		virtual void registerOutputs();
		virtual void registerInputs();

	protected:
		VectorSplitNodeOutput m_x;
		VectorSplitNodeOutput m_y;
		VectorSplitNodeOutput m_z;
		VectorSplitNodeOutput m_w;
	};

} /* namespace manta */

#endif /* VECTOR_SPLIT_NODE_H */

