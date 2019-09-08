#ifndef MANTARAY_MEMORY_MANAGEMENT_H
#define MANTARAY_MEMORY_MANAGEMENT_H

#include <assert.h>

namespace manta {

    typedef unsigned __int64 mem_size;

    constexpr mem_size KB = 1000;
    constexpr mem_size MB = 1000 * KB;
    constexpr mem_size GB = 1000 * MB;

#define CHECK_ALIGNMENT(pointer, required) assert((((unsigned __int64)((char *)(pointer))) % (required)) == 0)

} /* namespace manta */

#endif /* MANTARAY_MEMORY_MANAGEMENT_H */
