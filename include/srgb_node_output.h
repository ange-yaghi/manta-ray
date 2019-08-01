#ifndef MANTARAY_SRGB_NODE_OUTPUT_H
#define MANTARAY_SRGB_NODE_OUTPUT_H

#include "vector_node_output.h"

#include "intersection_point.h"

namespace manta {

	class SrgbNodeOutput : public VectorNodeOutput {
	public:
		SrgbNodeOutput();
		virtual ~SrgbNodeOutput();

		virtual void sample(const IntersectionPoint *surfaceInteraction, void *target) const;
		virtual void discreteSample2D(int x, int y, void *target) const;
		virtual void fullOutput(const void **target) const;

		piranha::pNodeInput *getRConnection() { return &m_r; }
        piranha::pNodeInput *getGConnection() { return &m_g; }
        piranha::pNodeInput *getBConnection() { return &m_b; }
        piranha::pNodeInput *getAConnection() { return &m_a; }

		virtual void registerInputs();

	protected:
        piranha::pNodeInput m_r;
        piranha::pNodeInput m_g;
        piranha::pNodeInput m_b;
        piranha::pNodeInput m_a;
	};

} /* namespace manta */

#endif /* MANTARAY_SRGB_NODE_OUTPUT_H */
