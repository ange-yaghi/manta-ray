#ifndef PIRANHA_FLOAT_NODE_OUTPUT_H
#define PIRANHA_FLOAT_NODE_OUTPUT_H

#include "node_output.h"

namespace piranha {

	class FloatNodeOutput: public NodeOutput {
	protected:
		static const NodeType FloatType;

	public:
		FloatNodeOutput() : NodeOutput(&FloatType) {
			/* void */
		}

		virtual ~FloatNodeOutput() {
			/* void */
		}
	};

} /* namespace piranha */

#endif /* PIRANHA_FLOAT_NODE_OUTPUT_H */
