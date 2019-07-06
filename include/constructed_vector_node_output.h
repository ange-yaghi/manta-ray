#ifndef CONSTRUCTED_VECTOR_NODE_OUTPUT_H
#define CONSTRUCTED_VECTOR_NODE_OUTPUT_H

#include <vector_node_output.h>

namespace manta {

	class ConstructedVectorNodeOutput : public VectorNodeOutput {
	public:
		ConstructedVectorNodeOutput();
		virtual ~ConstructedVectorNodeOutput();

		virtual void sample(const IntersectionPoint *surfaceInteraction, void *target) const;
		virtual void discreteSample2D(int x, int y, void *target) const;
		virtual void fullOutput(const void **target) const;

		pNodeInput *getXConnection() { return &m_x; }
		pNodeInput *getYConnection() { return &m_y; }
		pNodeInput *getZConnection() { return &m_z; }
		pNodeInput *getWConnection() { return &m_w; }

		const NodeOutput *getXOutput() { return m_x; }

		void registerInputs();

	protected:
		pNodeInput m_x;
		pNodeInput m_y;
		pNodeInput m_z;
		pNodeInput m_w;
	};

} /* namespace manta */

#endif /* CONSTRUCTED_VECTOR_NODE_OUTPUT_H */