#ifndef MANTARAY_COMPLEX_NODE_OUTPUT_H
#define MANTARAY_COMPLEX_NODE_OUTPUT_H

#include "streaming_node_output.h"

#include "manta_math.h"
#include "vector_map_2d.h"

namespace manta {

	class ComplexNodeOutput : public StreamingNodeOutput {
	public:
		static const piranha::ChannelType ComplexType;

	public:
		ComplexNodeOutput();
		ComplexNodeOutput(const piranha::ChannelType *channelType);
		virtual ~ComplexNodeOutput();

	protected:
		virtual piranha::Node *newInterface(piranha::NodeAllocator *nodeAllocator);
	};

} /* namespace manta */

#endif /* MANTARAY_COMPLEX_NODE_OUTPUT_H */
