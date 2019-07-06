#ifndef CONSTRUCTED_FLOAT_NODE_H
#define CONSTRUCTED_FLOAT_NODE_H

#include <node.h>

#include <constructed_float_node_output.h>

namespace manta {

	class ConstructedFloatNode : public Node {
	public:
		ConstructedFloatNode();
		virtual ~ConstructedFloatNode();

	protected:
		virtual void _initialize();
		virtual void _evaluate();
		virtual void _destroy();

		virtual void registerOutputs();
		virtual void registerInputs();

	protected:
		ConstructedFloatNodeOutput m_output;
	};

} /* namespace manta */

#endif /* CONSTRUCTED_FLOAT_NODE_H */
