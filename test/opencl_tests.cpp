#include <pch.h>

#include <gpu_manager_opencl.h>
#include <gpu_kernel_opencl.h>
#include <gpu_memory_opencl.h>

using namespace manta;

TEST(OpenCLTests, OpenCLSanityCheck) {
	GPUManagerOpenCL gpuManager;
	gpuManager.initialize("../../../opencl_programs/mantaray.cl");
	
	GPUMemory *memoryBuffer1 = gpuManager.createMemoryBuffer(sizeof(float) * 10, GPUMemory::READ_ONLY);
	GPUMemory *memoryBuffer2 = gpuManager.createMemoryBuffer(sizeof(float) * 10, GPUMemory::READ_ONLY);
	GPUMemory *memoryBuffer3 = gpuManager.createMemoryBuffer(sizeof(float) * 10, GPUMemory::WRITE_ONLY);

	float *testData = new float[10];
	float *testData2 = new float[10];

	for (int i = 0; i < 10; i++) {
		testData[i] = i;
		testData2[i] = 2 * i;
	}

	memoryBuffer1->write(testData);
	memoryBuffer2->write(testData2);

	GPUKernel *kernel = gpuManager.createKernel("test_kernel");
	kernel->setArgument(0, memoryBuffer1);
	kernel->setArgument(1, memoryBuffer2);
	kernel->setArgument(2, memoryBuffer3);
	kernel->run(10);

	float *data = new float[10];
	memoryBuffer3->read((void *)data);

	for (int i = 0; i < 10; i++) {
		EXPECT_NEAR(testData[i] + 2 * testData2[i], data[i], 1e-5);
	}
}
