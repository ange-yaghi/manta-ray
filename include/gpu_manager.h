#ifndef GPU_MANAGER_H
#define GPU_MANAGER_H

#include <gpu_memory.h>

namespace manta {

	class GPUKernel;

	class GPUManager {
	public:
		GPUManager() {}
		~GPUManager() {}

		virtual void initialize(const char *programLocation) = 0;
		virtual void destroy() = 0;

		virtual GPUMemory *createMemoryBuffer(unsigned int size, GPUMemory::MODE mode) = 0;
		virtual GPUKernel *createKernel(const char *name, const void *optionalData = nullptr) = 0;

		virtual const char *getDeviceName() const = 0;
	};

} /* namespace manta */

#endif /* GPU_MANAGER_H */
