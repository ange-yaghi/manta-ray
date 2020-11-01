#ifndef PIRANHA_MEMORY_TRACKER_H
#define PIRANHA_MEMORY_TRACKER_H

#include "build_settings.h"

#include <string>
#include <vector>

namespace piranha {

    class MemoryTracker {
    public:
        struct Allocation {
            std::string filename;
            int line;
            int index;

            void *address = nullptr;
            bool freed = false;
        };

    protected:
        static MemoryTracker *s_instance;

    public:
        MemoryTracker();
        ~MemoryTracker();

        static MemoryTracker *get();
        void reset();

        bool find(void *address, Allocation *allocation);

        void recordAllocation(void *address, const std::string &filename, int line);
        void recordFree(void *address);

        int countLeaks() const;

    private:
        std::vector<Allocation> m_allocations;
    };

#if ENABLE_MEMORY_TRACKER

#define TRACK(address) \
    (trackedAllocation(address, __FILE__, __LINE__))

#define FTRACK(address) \
    (trackedFree(address))

#else

#define TRACK(address) (address)
#define FTRACK(address) (address)

#endif /* ENABLE_MEMORY_TRACKER */

    template <typename T>
    T *trackedAllocation(T *address, const char *_filename, int line) {
        const std::string filename = _filename;
        MemoryTracker::get()->recordAllocation(address, filename, line);

        return address;
    }

    template <typename T>
    T *trackedFree(T *address) {
        MemoryTracker::get()->recordFree(address);

        return address;
    }

} /* namespace piranha */

#endif /* PIRANHA_MEMORY_TRACKER_H */
