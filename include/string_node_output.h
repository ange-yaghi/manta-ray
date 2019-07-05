#ifndef STRING_NODE_OUTPUT_H
#define STRING_NODE_OUTPUT_H

#include <node_output.h>

namespace manta {

	class StringNodeOutput : public NodeOutput {
	protected:
		static const NodeType StringType;

	public:
		StringNodeOutput();
		virtual ~StringNodeOutput();
	};

} /* namespace manta */

#endif /* STRING_NODE_OUTPUT_H */
