#ifndef MANTARAY_STRING_NODE_OUTPUT_H
#define MANTARAY_STRING_NODE_OUTPUT_H

#include <piranha.h>

namespace manta {

	class StringNodeOutput : public piranha::NodeOutput {
	public:
		static const piranha::ChannelType StringType;

	public:
		StringNodeOutput();
		virtual ~StringNodeOutput();
	};

} /* namespace manta */

#endif /* MANTARAY_STRING_NODE_OUTPUT_H */
