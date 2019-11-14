#ifndef MANTARAY_NODE_CACHE_H
#define MANTARAY_NODE_CACHE_H

#include "node.h"

#include "stack_allocator.h"
#include "intersection_point.h"

namespace manta {

    typedef long long cache_key;

    template <typename T_Memory>
    class NodeCache {
    public:
        static constexpr int CACHE_DEPTH = 1;
        static constexpr int MAX_THREADS = 64;

    public:
        struct CacheMemory {
            intersection_id intersectionId;
            T_Memory memory;
        };

        struct ThreadMemory {
            CacheMemory memory[CACHE_DEPTH];
            int currentSlot;
        };

    public:
        NodeCache() {
            for (int i = 0; i < MAX_THREADS; i++) {
                m_cache[i].currentSlot = 0;
                for (int j = 0; j < CACHE_DEPTH; j++) {
                    m_cache[i].memory[j].intersectionId = INVALID_CACHE_KEY;
                }
            }
        }

        virtual ~NodeCache() {
            /* void */
        }

        inline const T_Memory *cacheGet(cache_key key, int threadId) const {
            const ThreadMemory &threadMemory = m_cache[threadId];
            for (int i = 0; i < CACHE_DEPTH; i++) {
                if (threadMemory.memory[i].intersectionId == key) {
                    return &threadMemory.memory[i].memory;
                }
            }

            return nullptr;
        }

        inline T_Memory *cachePut(cache_key key, int threadId) {
            ThreadMemory &threadMemory = m_cache[threadId];
            CacheMemory &memory = threadMemory.memory[threadMemory.currentSlot];
            memory.intersectionId = key;

            threadMemory.currentSlot = (threadMemory.currentSlot + 1) % CACHE_DEPTH;

            memory.memory = T_Memory();

            return &memory.memory;
        }

    protected:
        ThreadMemory m_cache[MAX_THREADS];
    };

} /* namespace manta */

#endif /* MANTARAY_NODE_CACHE_H */
