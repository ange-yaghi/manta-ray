#ifndef GPU_MANAGER_OPENCL_H
#define GPU_MANAGER_OPENCL_H

#include <gpu_manager.h>

#include <CL/opencl.h>

#include <vector>

namespace manta {

	class GPUMemoryOpenCL;
	class GPUKernelOpenCL;
	class GPUKernel;

	class GPUManagerOpenCL : public GPUManager {
	public:
		GPUManagerOpenCL();
		~GPUManagerOpenCL();

		virtual void initialize(const char *programLocation);

		virtual GPUMemory *createMemoryBuffer(unsigned int size, GPUMemory::MODE mode);
		virtual GPUKernel *createKernel(const char *name, const void *optionalData = nullptr);
		virtual const char *getDeviceName() const;

		virtual void destroy();
		
		// OpenCL functions
		cl_int writeToBuffer(GPUMemoryOpenCL *buffer, const void *data);
		cl_int readFromBuffer(GPUMemoryOpenCL *buffer, void *target);

		cl_int setArgument(GPUKernelOpenCL *kernel, GPUMemory *memory, int argument);
		cl_int runKernel(GPUKernel *kernel, unsigned int elements);

	protected:
		cl_platform_id m_platformId;
		cl_device_id m_deviceId;
		cl_uint m_numDevices;
		cl_uint m_numPlatforms;

		cl_program m_program;

		cl_context m_context;
		cl_command_queue m_commandQueue;		

		std::vector<GPUMemory *>m_memoryBuffers;
		std::vector<GPUKernel *>m_kernels;

		char m_deviceName[128];
	};

} /* namespace manta */

#endif /* GPU_MANAGER_OPENCL_H */
