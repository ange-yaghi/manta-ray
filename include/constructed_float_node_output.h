#ifndef MANTARAY_CONSTRUCTED_FLOAT_NODE_OUTPUT_H
#define MANTARAY_CONSTRUCTED_FLOAT_NODE_OUTPUT_H

#include "float_node_output.h"

#include "intersection_point.h"

namespace manta {

	class ConstructedFloatNodeOutput : public FloatNodeOutput {
	public:
		ConstructedFloatNodeOutput();
		virtual ~ConstructedFloatNodeOutput();

		virtual void sample(const IntersectionPoint *surfaceInteraction, void *target) const;
		virtual void discreteSample2D(int x, int y, void *target) const;
		virtual void fullCompute(void *target) const;

		virtual void registerInputs();

		piranha::pNodeInput *getInputConnection() { return &m_input; }

	protected:
        piranha::pNodeInput m_input;
	};

} /* namespace manta */

#endif /* MANTARAY_CONSTRUCTED_FLOAT_NODE_OUTPUT_H */
