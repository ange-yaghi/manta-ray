#include <material_node.h>

#include <stack_allocator.h>

manta::MaterialNode::MaterialNode() {
	m_id = -1;
	m_name = "";
}

manta::MaterialNode::~MaterialNode() {

}

void manta::MaterialNode::initialize(const IntersectionPoint *surfaceInteraction, MaterialNodeMemory *memory, StackAllocator *stackAllocator) const {
	memset(memory, 0, sizeof(MaterialNodeMemory));
}

void manta::MaterialNode::free(MaterialNodeMemory *memory, StackAllocator *stackAllocator) const {
	if (memory->extraMemory != nullptr) {
		stackAllocator->free(memory->extraMemory);
		memory->extraMemory = nullptr;
	}
}
