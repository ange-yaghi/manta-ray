#ifndef MANTARAY_VECTOR_SPLIT_NODE_H
#define MANTARAY_VECTOR_SPLIT_NODE_H

#include <piranha.h>

#include "vector_split_node_output.h"

namespace manta {

	class VectorSplitNode : public piranha::Node {
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
        piranha::pNodeInput m_input;

		VectorSplitNodeOutput m_x;
		VectorSplitNodeOutput m_y;
		VectorSplitNodeOutput m_z;
		VectorSplitNodeOutput m_w;
	};

} /* namespace manta */

#endif /* MANTARAY_VECTOR_SPLIT_NODE_H */

