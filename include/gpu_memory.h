#ifndef GPU_MEMORY_H
#define GPU_MEMORY_H

#include <manta_build_conf.h>
#if INCLUDE_OPENCL_IMPL

namespace manta {

	class GPUMemory {
	public:
		enum MODE {
			READ_ONLY,
			WRITE_ONLY
		};
	public:
		GPUMemory() {}
		virtual ~GPUMemory() {}

		virtual void write(const void *data) = 0;
		virtual void read(void *target) = 0;

		virtual void free() = 0;

		virtual unsigned int getSize() const = 0;
	};

} /* namespace manta */

#endif /* INCLUDE_OPENCL_IMPL */
#endif /* GPU_MEMORY_H */
