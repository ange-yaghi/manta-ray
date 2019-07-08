#ifndef PIRANHA_CONSTRUCTED_FLOAT_NODE_H
#define PIRANHA_CONSTRUCTED_FLOAT_NODE_H

#include "node.h"

#include "constructed_float_node_output.h"

namespace piranha {

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

} /* namespace piranha */

#endif /* PIRANHA_CONSTRUCTED_FLOAT_NODE_H */
