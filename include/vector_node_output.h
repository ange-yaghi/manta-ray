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
        VectorNodeOutput(const piranha::ChannelType *channelType);
		virtual ~VectorNodeOutput();

		void calculateAllDimensions(VectorMap2D *target) const;

	protected:
		virtual piranha::Node *newInterface(piranha::NodeAllocator *nodeAllocator);

	protected:
		bool m_scalar;
	};

} /* namespace manta */

#endif /* MANTARAY_VECTOR_NODE_OUTPUT_H */
