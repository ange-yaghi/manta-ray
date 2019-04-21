#include <node.h>

#include <stack_allocator.h>

manta::Node::Node() {
	m_id = -1;
	m_name = "";

	m_initialized = false;
	m_evaluated = false;
}

manta::Node::~Node() {

}

void manta::Node::initialize() {
	if (isInitialized()) return;

	// Set initialized flag
	m_initialized = true;

	registerDependencies();

	// Initialize
	_initialize();
}

void manta::Node::evaluate() {
	if (isEvaluated()) return;

	// Set evaluated flag
	m_evaluated = true;

	// First evaluate all dependencies
	int dependencyCount = getDependencyCount();

	for (int i = 0; i < dependencyCount; i++) {
		Node **node = m_dependencies[i].node;
		if (node != nullptr) {
			(*node)->evaluate();
		}
	}

	// Node can now self-evaluate
	_evaluate();
}

void manta::Node::destroy() {
	m_initialized = false;
	m_evaluated = false;

	_destroy();
}

void manta::Node::initializeSessionMemory(const IntersectionPoint *surfaceInteraction, NodeSessionMemory *memory, StackAllocator *stackAllocator) const {
	memset(memory, 0, sizeof(NodeSessionMemory));
}

void manta::Node::destroySessionMemory(NodeSessionMemory *memory, StackAllocator *stackAllocator) const {
	if (memory->extraMemory != nullptr) {
		stackAllocator->free(memory->extraMemory);
		memory->extraMemory = nullptr;
	}
}

void manta::Node::connectInput(Node **node, const char *name) {
	int dependencyCount = getDependencyCount();

	for (int i = 0; i < dependencyCount; i++) {
		const char *inputName = m_dependencies[i].name;
		if (strcmp(inputName, name) == 0) {
			m_dependencies[i].node = node;
			break;
		}
	}

}

void manta::Node::_initialize() {
	/* void */
}

void manta::Node::_evaluate() {
	/* void */
}

void manta::Node::_destroy() {
	/* void */
}

void manta::Node::registerDependencies() {
	/* void */
}
