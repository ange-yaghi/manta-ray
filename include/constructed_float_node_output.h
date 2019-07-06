#ifndef CONSTRUCTED_FLOAT_NODE_OUTPUT_H
#define CONSTRUCTED_FLOAT_NODE_OUTPUT_H

#include <float_node_output.h>

#include <constructed_float_node_output.h>

namespace manta {

	class ConstructedFloatNodeOutput : public FloatNodeOutput {
	public:
		ConstructedFloatNodeOutput();
		virtual ~ConstructedFloatNodeOutput();

		virtual void sample(const IntersectionPoint *surfaceInteraction, void *target) const;
		virtual void discreteSample2D(int x, int y, void *target) const;
		virtual void fullCompute(void *target) const;

		virtual void registerInputs();

		pNodeInput *getInputConnection() { return &m_input; }

	protected:
		pNodeInput m_input;
	};

} /* namespace manta */

#endif /* CONSTRUCTED_FLOAT_NODE_OUTPUT_H */
