#include <node.h>

#include <node_output.h>
#include <stack_allocator.h>
#include <node_program.h>

manta::Node::Node() {
	m_id = -1;
	m_name = "";

	m_initialized = false;
	m_evaluated = false;
	m_primaryOutput = nullptr;
}

manta::Node::~Node() {
	/* void */
}

void manta::Node::initialize() {
	if (isInitialized()) return;

	// Set initialized flag
	m_initialized = true;

	registerInputs();
	registerOutputs();

	// Initialize
	_initialize();
}

void manta::Node::evaluate() {
	if (isEvaluated()) return;

	// Set evaluated flag
	m_evaluated = true;

	// First evaluate all dependencies
	int inputCount = getInputCount();
	for (int i = 0; i < inputCount; i++) {
		pNodeInput *node = m_inputs[i].input;
		if (node != nullptr && *node != nullptr) {
			(*node)->evaluate();
		}
	}

	if (requiresMaterials()) {
		getProgram()
			->getMaterialManager()
			->evaluateAllMaterialNodes();
	}

	// Node can now self-evaluate
	_evaluate();

	// All dependencies are evaluated so outputs can now determine their
	// dimensions
	int outputCount = getOutputCount();
	for (int i = 0; i < outputCount; i++) {
		NodeOutput *node = m_outputs[i].output;
		node->evaluateDimensions();
	}

	int outputReferenceCount = getOutputReferenceCount();
	for (int i = 0; i < outputReferenceCount; i++) {
		NodeOutput *node = *m_outputReferences[i].output;
		node->evaluate();
	}
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

void manta::Node::connectInput(pNodeInput input, const std::string &name) {
	int inputCount = getInputCount();

	for (int i = 0; i < inputCount; i++) {
		if (name == m_inputs[i].name) {
			*m_inputs[i].input = input;
			
			// Warning: do not break here! There could potentially be multiple
			// inputs with the same name referencing different endpoints
		}
	}
}

manta::NodeOutput *manta::Node::getPrimaryOutput() const {
	return m_primaryOutput;
}

manta::NodeOutput *manta::Node::getOutput(const std::string &name) const {
	int outputCount = getOutputCount();

	for (int i = 0; i < outputCount; i++) {
		if (name == m_outputs[i].name) {
			return m_outputs[i].output;
		}
	}

	int outputReferenceCount = getOutputReferenceCount();
	for (int i = 0; i < outputReferenceCount; i++) {
		if (name == m_outputReferences[i].name) {
			return *m_outputReferences[i].output;
		}
	}

	return nullptr;
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

void manta::Node::registerInputs() {
	/* void */
}

void manta::Node::registerInput(pNodeInput *node, const std::string &name) {
	m_inputs.push_back({ node, name });
}

void manta::Node::registerOutput(NodeOutput *node, const std::string &name) {
	m_outputs.push_back({ node, name });
	node->setParentNode(this);
}

void manta::Node::setPrimaryOutput(NodeOutput *node) {
	m_primaryOutput = node;
}

void manta::Node::registerOutputReference(NodeOutput *const *node, const std::string &name) {
	m_outputReferences.push_back({ node, name });
}

void manta::Node::registerOutputs() {
	/* void */
}
