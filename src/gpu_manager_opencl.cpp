#include <manta_build_conf.h>
#if INCLUDE_OPENCL_IMPL

#include <gpu_manager_opencl.h>

#include <gpu_memory_opencl.h>
#include <gpu_kernel_opencl.h>

#define MAX_SOURCE_SIZE (0x100000)

manta::GPUManagerOpenCL::GPUManagerOpenCL() {

}

manta::GPUManagerOpenCL::~GPUManagerOpenCL() {

}

void manta::GPUManagerOpenCL::initialize(const char *programLocation) {
	m_platformId = NULL;
	m_deviceId = NULL;
	
	// Get platform and device information
	cl_int ret;
	ret = clGetPlatformIDs(1, &m_platformId, &m_numPlatforms);
	ret = clGetDeviceIDs(m_platformId, CL_DEVICE_TYPE_GPU, 1, &m_deviceId, &m_numDevices);
	ret = clGetDeviceInfo(m_deviceId, CL_DEVICE_NAME, sizeof(m_deviceName), (void *)m_deviceName, NULL);

	// Create an OpenCL context
	m_context = clCreateContext(NULL, 1, &m_deviceId, NULL, NULL, &ret);

	// Create the command queue
	m_commandQueue = clCreateCommandQueue(m_context, m_deviceId, 0, &ret);

	// Create the program
	FILE *fp;
	char *source_str;
	size_t source_size;

	fopen_s(&fp, programLocation, "r");
	if (!fp) {
		fprintf(stderr, "Failed to load program.\n");
		exit(1);
	}
	source_str = (char*)malloc(MAX_SOURCE_SIZE);
	source_size = fread(source_str, 1, MAX_SOURCE_SIZE, fp);
	fclose(fp);

	m_program = clCreateProgramWithSource(m_context, 1, (const char **)&source_str, (const size_t *)&source_size, &ret);
	ret = clBuildProgram(m_program, 1, &m_deviceId, NULL, NULL, NULL);

	if (ret == CL_BUILD_PROGRAM_FAILURE) {
		size_t size;
		ret = clGetProgramBuildInfo(m_program, m_deviceId, CL_PROGRAM_BUILD_LOG, NULL, NULL, &size);

		char *outputBuffer = new char[size + 1];
		ret = clGetProgramBuildInfo(m_program, m_deviceId, CL_PROGRAM_BUILD_LOG, size, (void *)outputBuffer, NULL);
		int a = 0;
	}
}

manta::GPUMemory *manta::GPUManagerOpenCL::createMemoryBuffer(unsigned int size, GPUMemory::MODE mode) {
	cl_int ret;
	
	cl_mem_flags flags;

	if (mode == GPUMemory::READ_ONLY) flags = CL_MEM_READ_ONLY;
	else if (mode == GPUMemory::WRITE_ONLY) flags = CL_MEM_WRITE_ONLY;

	cl_mem memObject = clCreateBuffer(
		m_context, 
		flags | CL_MEM_ALLOC_HOST_PTR,
		size, 
		NULL, 
		&ret);

	GPUMemoryOpenCL *newBuffer = new GPUMemoryOpenCL;
	newBuffer->setManager(this);
	newBuffer->setObject(memObject);
	newBuffer->setSize(size);

	m_memoryBuffers.push_back((GPUMemory *)newBuffer);

	return (manta::GPUMemory *)newBuffer;
}

manta::GPUKernel * manta::GPUManagerOpenCL::createKernel(const char *name, const void *optionalData) {
	cl_int result;
	cl_kernel kernel = clCreateKernel(m_program, name, &result);

	GPUKernelOpenCL *newKernel = new GPUKernelOpenCL;
	newKernel->setKernel(kernel);
	newKernel->setManager(this);

	m_kernels.push_back(newKernel);

	return newKernel;
}

const char *manta::GPUManagerOpenCL::getDeviceName() const {
	return m_deviceName;
}

void manta::GPUManagerOpenCL::destroy() {
	clFlush(m_commandQueue);
	clFinish(m_commandQueue);

	size_t kernelCount = m_kernels.size();
	for (size_t i = 0; i < kernelCount; i++) {
		m_kernels[i]->free();
		delete m_kernels[i];
	}

	clReleaseProgram(m_program);

	size_t memoryBufferCount = m_memoryBuffers.size();
	for (size_t i = 0; i < memoryBufferCount; i++) {
		m_memoryBuffers[i]->free();
		delete m_memoryBuffers[i];
	}

	clReleaseCommandQueue(m_commandQueue);
	clReleaseContext(m_context);
}

cl_int manta::GPUManagerOpenCL::writeToBuffer(GPUMemoryOpenCL *buffer, const void *data) {
	cl_int result = clEnqueueWriteBuffer(m_commandQueue, buffer->getObject(), CL_TRUE, 0, (size_t)buffer->getSize(), data, 0, NULL, NULL);
	return result;
}

cl_int manta::GPUManagerOpenCL::readFromBuffer(GPUMemoryOpenCL *buffer, void *target) {
	cl_int result = clEnqueueReadBuffer(m_commandQueue, buffer->getObject(), CL_TRUE, 0, (size_t)buffer->getSize(), target, 0, NULL, NULL);
	return result;
}

cl_int manta::GPUManagerOpenCL::setArgument(GPUKernelOpenCL * kernel, GPUMemory *_memory, int argument) {
	GPUMemoryOpenCL *memory = (GPUMemoryOpenCL *)_memory;

	cl_int result = clSetKernelArg(kernel->getKernel(), argument, sizeof(cl_mem), (void *)memory->getObject());
	return result;
}

cl_int manta::GPUManagerOpenCL::runKernel(GPUKernel *_kernel, unsigned int elements) {
	GPUKernelOpenCL *kernel = (GPUKernelOpenCL *)_kernel;
	size_t globalItemSize = (size_t)elements;
	size_t localItemSize = 64;

	if (localItemSize > elements) localItemSize = (size_t)elements;

	cl_int result = clEnqueueNDRangeKernel(m_commandQueue, kernel->getKernel(), 1, NULL, &globalItemSize, &localItemSize, 0, NULL, NULL);
	return result;
}

#endif /* INCLUDE_OPENCL_IMPL */
