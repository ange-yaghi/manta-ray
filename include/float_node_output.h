#ifndef MANTARAY_FLOAT_NODE_OUTPUT_H
#define MANTARAY_FLOAT_NODE_OUTPUT_H

#include <piranha.h>

namespace manta {

	class FloatNodeOutput: public piranha::NodeOutput {
	protected:
		static const piranha::ChannelType FloatType;

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

#endif /* MANTARAY_FLOAT_NODE_OUTPUT_H */
