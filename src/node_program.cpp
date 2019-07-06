#include <node_program.h>

#include <node.h>

manta::NodeProgram::NodeProgram() {
	/* void */
}

manta::NodeProgram::~NodeProgram() {
	/* void */
}

void manta::NodeProgram::addNode(Node *node) {
	node->setProgram(this);
	m_nodes.push_back(node);

	// This registers the node in case there is a node that
	// has a dependency on materials
	if (node->isMaterial()) {
		m_materialManager.addMaterialNode(node);
	}
}

void manta::NodeProgram::execute() {
	int nodeCount = getNodeCount();

	// Initialize all nodes
	for (int i = 0; i < nodeCount; i++) {
		Node *node = getNode(i);
		node->initialize();
	}

	for (int i = 0; i < nodeCount; i++) {
		Node *node = getNode(i);
		node->evaluate();
	}
}
