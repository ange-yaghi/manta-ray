#ifndef GPU_KERNEL_OPENCL_H
#define GPU_KERNEL_OPENCL_H

#include <manta_build_conf.h>
#if INCLUDE_OPENCL_IMPL

#include <gpu_kernel.h>

#include <CL/opencl.h>

namespace manta {

	// Forward declarations
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

#endif /* INCLUDE_OPENCL_IMPL */
#endif /* GPU_KERNEL_OPENCL_H */
