#ifndef CONSTRUCTED_VECTOR_NODE_H
#define CONSTRUCTED_VECTOR_NODE_H

#include <node.h>

#include <constructed_vector_node_output.h>

namespace manta {

	class ConstructedVectorNode : public Node {
	public:
		ConstructedVectorNode();
		virtual ~ConstructedVectorNode();

	protected:
		virtual void _initialize();
		virtual void _evaluate();
		virtual void _destroy();

		virtual void registerOutputs();
		virtual void registerInputs();

	protected:
		ConstructedVectorNodeOutput m_output;
	};

} /* namespace manta */

#endif /* CONSTRUCTED_VECTOR_NODE_H */
