#ifndef GPU_KERNEL_H
#define GPU_KERNEL_H

namespace manta {

	class GPUMemory;

	class GPUKernel {
	public:
		GPUKernel() {}
		virtual ~GPUKernel() {}

		virtual void setArgument(int argument, GPUMemory *memory) = 0;
		virtual void run(unsigned int elements) = 0;
		virtual void free() = 0;
	};

} /* namespace manta */

#endif /* GPU_KERNEL_H */
