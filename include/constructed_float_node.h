#ifndef MANTARAY_CONSTRUCTED_FLOAT_NODE_H
#define MANTARAY_CONSTRUCTED_FLOAT_NODE_H

#include <piranha.h>

#include "constructed_float_node_output.h"

namespace manta {

	class ConstructedFloatNode : public piranha::Node {
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

#endif /* MANTARAY_CONSTRUCTED_FLOAT_NODE_H */
