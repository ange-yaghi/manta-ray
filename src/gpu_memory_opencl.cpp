#include <gpu_memory_opencl.h>

#include <gpu_manager_opencl.h>

manta::GPUMemoryOpenCL::GPUMemoryOpenCL() {
	m_memoryObject = NULL;
}

manta::GPUMemoryOpenCL::~GPUMemoryOpenCL() {

}

void manta::GPUMemoryOpenCL::read(void *target) {
	m_manager->readFromBuffer(this, target);
}

void manta::GPUMemoryOpenCL::write(const void *data) {
	m_manager->writeToBuffer(this, data);
}

void manta::GPUMemoryOpenCL::free() {
	clReleaseMemObject(m_memoryObject);
}
