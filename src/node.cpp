#include "../include/node.h"

#include "../include/path.h"

manta::Node::Node() {
    /* void */
}

manta::Node::~Node() {
    /* void */
}

void manta::Node::initializeSessionMemory(
    const IntersectionPoint *surfaceInteraction, NodeSessionMemory *memory, 
    StackAllocator *stackAllocator) const
{
    memset(memory, 0, sizeof(NodeSessionMemory));
}

void manta::Node::destroySessionMemory(NodeSessionMemory *memory, 
    StackAllocator *stackAllocator) const 
{
    if (memory->extraMemory != nullptr) {
        stackAllocator->free(memory->extraMemory);
        memory->extraMemory = nullptr;
    }
}

void manta::Node::resolvePath(const Path *relative, Path *target) const {
    if (relative->isAbsolute()) {
        *target = *relative;
        return;
    }

    piranha::Path unitFile = getProgram()->getRootUnit()->getPath();
    piranha::Path home;
    
    unitFile.getParentPath(&home);

    *target = Path(home.toString()).append(*relative);
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
