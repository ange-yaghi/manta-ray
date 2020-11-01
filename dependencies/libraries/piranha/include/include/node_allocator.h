#ifndef PIRANHA_NODE_ALLOCATOR_H
#define PIRANHA_NODE_ALLOCATOR_H

#include "memory_tracker.h"

namespace piranha {

    class NodeAllocator {
    public:
        NodeAllocator();
        virtual ~NodeAllocator();

        template <typename T_NodeType>
        T_NodeType *allocate() {
            void *memory = allocate(sizeof(T_NodeType));
            return TRACK(new(memory) T_NodeType);
        }

        template <typename T_NodeType>
        void free(T_NodeType *target) {
            FTRACK(target);

            target->~T_NodeType();
            free((void *)target);
        }

    protected:
        virtual void *allocate(int size);
        virtual void free(void *memory);
    };

} /* namespace piranha */

#endif /* PIRANHA_NODE_ALLOCATOR_H */
