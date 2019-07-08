#ifndef PIRANHA_CONSTRUCTED_FLOAT_NODE_OUTPUT_H
#define PIRANHA_CONSTRUCTED_FLOAT_NODE_OUTPUT_H

#include "float_node_output.h"

#include "constructed_float_node_output.h"

namespace piranha {

	class ConstructedFloatNodeOutput : public FloatNodeOutput {
	public:
		ConstructedFloatNodeOutput();
		virtual ~ConstructedFloatNodeOutput();

		virtual void fullCompute(void *target) const;
		virtual void registerInputs();

		pNodeInput *getInputConnection() { return &m_input; }

	protected:
		pNodeInput m_input;
	};

} /* namespace piranha */

#endif /* PIRANHA_CONSTRUCTED_FLOAT_NODE_OUTPUT_H */
