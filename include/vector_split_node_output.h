#ifndef MANTARAY_VECTOR_SPLIT_NODE_OUTPUT_H
#define MANTARAY_VECTOR_SPLIT_NODE_OUTPUT_H

#include "vector_node_output.h"

#include "intersection_point.h"

namespace manta {

	class VectorSplitNodeOutput : public VectorNodeOutput {
	public:
		VectorSplitNodeOutput();
		virtual ~VectorSplitNodeOutput();

		void setValueIndex(int valueIndex) { m_valueIndex = valueIndex; }
		int getValueIndex() const { return m_valueIndex; }

		virtual void sample(const IntersectionPoint *surfaceInteraction, void *target) const;
		virtual void discreteSample2D(int x, int y, void *target) const;
		virtual void fullOutput(const void **target) const;

		piranha::pNodeInput *getInputConnection() { return &m_input; }

		virtual void registerInputs();

	protected:
		piranha::pNodeInput m_input;
		int m_valueIndex;
	};

} /* namespace manta */

#endif /* MANTARAY_CONSTRUCTED_VECTOR_NODE_OUTPUT_H */
