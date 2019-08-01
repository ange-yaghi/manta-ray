#ifndef MANTARAY_NODE_H
#define MANTARAY_NODE_H

#include <piranha.h>

#include "stack_allocator.h"

namespace manta {

    struct IntersectionPoint;
    class Path;

	class Node : public piranha::Node {
	public:
        struct NodeSessionMemory {
            unsigned char memory[64];
            void *extraMemory;
        };

    public:
        Node();
        virtual ~Node();

        virtual void initializeSessionMemory(const IntersectionPoint *surfaceInteraction,
            NodeSessionMemory *memory, StackAllocator *stackAllocator) const;
        void destroySessionMemory(NodeSessionMemory *memory, StackAllocator *stackAllocator) const;

        void resolvePath(const Path *relative, Path *target) const;
	};

} /* namespace manta */

#endif /* MANTARAY_NODE_H */
