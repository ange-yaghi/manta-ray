#ifndef GPU_MEMORY_OPENCL_H
#define GPU_MEMORY_OPENCL_H

#include <manta_build_conf.h>
#if INCLUDE_OPENCL_IMPL

#include <gpu_memory.h>

#include <CL/opencl.h>

namespace manta {

	class GPUManagerOpenCL;

	class GPUMemoryOpenCL : GPUMemory {
	public:
		GPUMemoryOpenCL();
		~GPUMemoryOpenCL();

		virtual void read(void *target);
		virtual void write(const void *data);
		virtual void free();

		virtual unsigned int getSize() const { return m_size; }

		void setManager(GPUManagerOpenCL *manager) { m_manager = manager; }
		void setSize(unsigned int size) { m_size = size; }

		virtual void setObject(cl_mem memoryObject) { m_memoryObject = memoryObject; }
		cl_mem getObject() const { return m_memoryObject; }

	protected:
		GPUManagerOpenCL *m_manager;
		unsigned int m_size;

		cl_mem m_memoryObject;
	};

} /* namespace manta */

#endif /* INCLUDE_OPENCL_IMPL */
#endif /* GPU_MEMORY_OPENCL_H */
