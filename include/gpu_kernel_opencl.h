#ifndef GPU_KERNEL_OPENCL_H
#define GPU_KERNEL_OPENCL_H

#include <gpu_kernel.h>

#include <CL/opencl.h>

namespace manta {

	class GPUManagerOpenCL;

	class GPUKernelOpenCL : public GPUKernel {
	public:
		GPUKernelOpenCL();
		~GPUKernelOpenCL();

		virtual void setArgument(int argument, GPUMemory *memory);
		virtual void run(unsigned int elements);
		virtual void free();

		// OpenCL functions
		void setKernel(cl_kernel kernel) { m_kernel = kernel; }
		cl_kernel getKernel() const { return m_kernel; }

		void setManager(GPUManagerOpenCL *manager) { m_manager = manager; }
		GPUManagerOpenCL *getManager() { return m_manager; }

	protected:
		cl_kernel m_kernel;
		GPUManagerOpenCL *m_manager;
	};

} /* namespace manta */

#endif /* GPU_KERNEL_OPENCL_H */
