#include <manta_build_conf.h>
#if INCLUDE_OPENCL_IMPL

#include <gpu_kernel_opencl.h>

#include <gpu_manager_opencl.h>
#include <gpu_memory_opencl.h>

manta::GPUKernelOpenCL::GPUKernelOpenCL() {
	m_kernel = NULL;
	m_manager = nullptr;
}

manta::GPUKernelOpenCL::~GPUKernelOpenCL() {

}

void manta::GPUKernelOpenCL::setArgument(int argument, GPUMemory *_memory) {
	GPUMemoryOpenCL *memory = (GPUMemoryOpenCL *)_memory;
	cl_mem memObject = memory->getObject();
	cl_int result = clSetKernelArg(m_kernel, argument, sizeof(cl_mem), (void *)&memObject);
}

void manta::GPUKernelOpenCL::run(unsigned int elements) {
	cl_int result = m_manager->runKernel(
		static_cast<GPUKernel *>(this), 
		elements);
}

void manta::GPUKernelOpenCL::free() {
	cl_int result = clReleaseKernel(m_kernel);
}

#endif /* INCLUDE_OPENCL_IMPL */
