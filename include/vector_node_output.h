#ifndef MANTARAY_VECTOR_NODE_OUTPUT_H
#define MANTARAY_VECTOR_NODE_OUTPUT_H

#include "streaming_node_output.h"

#include "manta_math.h"
#include "vector_map_2d.h"

namespace manta {

	class VectorNodeOutput : public StreamingNodeOutput {
	public:
		static const piranha::ChannelType VectorType;

	public:
		VectorNodeOutput(bool scalar = false);
		virtual ~VectorNodeOutput();

		virtual void fullCompute(void *target) const;

	protected:
		virtual piranha::Node *generateInterface();

	protected:
		bool m_scalar;
	};

} /* namespace manta */

#endif /* MANTARAY_VECTOR_NODE_OUTPUT_H */
