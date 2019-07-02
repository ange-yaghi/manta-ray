#include <string_node_output.h>

#include <string>

const manta::NodeType manta::StringNodeOutput::StringType("StringNodeType");

manta::StringNodeOutput::StringNodeOutput() : NodeOutput(&StringType) {
	/* void */
}

manta::StringNodeOutput::~StringNodeOutput() {
	/* void */
}
