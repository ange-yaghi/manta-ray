#ifndef MEMORY_MANAGEMENT_H
#define MEMORY_MANAGEMENT_H

namespace manta {

	typedef unsigned __int64 mem_size;

	constexpr mem_size KB = 1000;
	constexpr mem_size MB = 1000 * KB;
	constexpr mem_size GB = 1000 * MB;

} /* namespace manta */

#endif /* MEMORY_MANAGEMENT_H */
