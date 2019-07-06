#ifndef SRGB_NODE_OUTPUT_H
#define SRGB_NODE_OUTPUT_H

#include <vector_node_output.h>

namespace manta {

	class SrgbNodeOutput : public VectorNodeOutput {
	public:
		SrgbNodeOutput();
		virtual ~SrgbNodeOutput();

		virtual void sample(const IntersectionPoint *surfaceInteraction, void *target) const;
		virtual void discreteSample2D(int x, int y, void *target) const;
		virtual void fullOutput(const void **target) const;

		pNodeInput *getRConnection() { return &m_r; }
		pNodeInput *getGConnection() { return &m_g; }
		pNodeInput *getBConnection() { return &m_b; }
		pNodeInput *getAConnection() { return &m_a; }

		virtual void registerInputs();

	protected:
		pNodeInput m_r;
		pNodeInput m_g;
		pNodeInput m_b;
		pNodeInput m_a;
	};

} /* namespace manta */

#endif /* SRGB_NODE_OUTPUT_H */
