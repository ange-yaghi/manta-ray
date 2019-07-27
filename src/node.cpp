#include "../include/node.h"

manta::Node::Node() {
    /* void */
}

manta::Node::~Node() {
    /* void */
}

void manta::Node::initializeSessionMemory(
    const IntersectionPoint *surfaceInteraction, NodeSessionMemory *memory, StackAllocator *stackAllocator) const
{
    memset(memory, 0, sizeof(NodeSessionMemory));
}

void manta::Node::destroySessionMemory(NodeSessionMemory *memory, StackAllocator *stackAllocator) const
{
    if (memory->extraMemory != nullptr) {
        stackAllocator->free(memory->extraMemory);
        memory->extraMemory = nullptr;
    }
}
