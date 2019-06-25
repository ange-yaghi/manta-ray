#include <node_program.h>

#include <node.h>

manta::NodeProgram::NodeProgram() {
	/* void */
}

manta::NodeProgram::~NodeProgram() {
	/* void */
}

void manta::NodeProgram::execute() {
	int nodeCount = getNodeCount();

	// Initialize all nodes
	for (int i = 0; i < nodeCount; i++) {
		Node *node = getNode(i);
		node->initialize();
	}

	for (int i = 0; i < nodeCount; i++) {

	}
}
