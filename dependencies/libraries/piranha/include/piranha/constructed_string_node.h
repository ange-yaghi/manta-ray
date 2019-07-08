#ifndef PIRANHA_CONSTRUCTED_STRING_NODE_H
#define PIRANHA_CONSTRUCTED_STRING_NODE_H

#include "node.h"

#include "constructed_string_node_output.h"

namespace piranha {

	class ConstructedStringNode : public Node {
	public:
		ConstructedStringNode();
		virtual ~ConstructedStringNode();

	protected:
		virtual void _initialize();
		virtual void _evaluate();
		virtual void _destroy();

		virtual void registerOutputs();
		virtual void registerInputs();

	protected:
		ConstructedStringNodeOutput m_output;
	};

} /* namespace piranha */

#endif /* PIRANHA_CONSTRUCTED_STRING_NODE_H */
