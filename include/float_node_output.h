#ifndef FLOAT_NODE_OUTPUT_H
#define FLOAT_NODE_OUTPUT_H

#include <node_output.h>

namespace manta {

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

		virtual void fullCompute(void *target) const;
	};

} /* namespace manta */

#endif /* FLOAT_NODE_OUTPUT_H */
