#include <pch.h>

#include <gpu_manager_opencl.h>
#include <gpu_kernel_opencl.h>
#include <gpu_memory_opencl.h>
#include <obj_file_loader.h>
#include <mesh.h>
#include <light_ray.h>
#include <intersection_point.h>

#include <chrono>
#include <fstream>

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

TEST(OpenCLTests, OpenCLPerformanceTest) {
	int testSize = 262144;

	GPUManagerOpenCL gpuManager;
	gpuManager.initialize("../../../opencl_programs/mantaray.cl");

	GPUMemory *memoryBuffer1 = gpuManager.createMemoryBuffer(sizeof(float) * testSize, GPUMemory::READ_ONLY);
	GPUMemory *memoryBuffer2 = gpuManager.createMemoryBuffer(sizeof(float) * testSize, GPUMemory::READ_ONLY);
	GPUMemory *memoryBuffer3 = gpuManager.createMemoryBuffer(sizeof(float) * testSize, GPUMemory::WRITE_ONLY);

	float *testData = new float[testSize];
	float *testData2 = new float[testSize];

	for (int i = 0; i < testSize; i++) {
		testData[i] = i;
		testData2[i] = 2 * i;
	}

	memoryBuffer1->write(testData);
	memoryBuffer2->write(testData2);

	GPUKernel *kernel = gpuManager.createKernel("test_kernel_sqrt");

	float *data = new float[testSize];

	auto beginOpenCL = std::chrono::high_resolution_clock::now();

	kernel->setArgument(0, memoryBuffer1);
	kernel->setArgument(1, memoryBuffer2);
	kernel->setArgument(2, memoryBuffer3);

	kernel->run(testSize);

	memoryBuffer3->read((void *)data);

	for (int i = 0; i < testSize; i++) {
		if (abs(sqrt((testData[i] + 2 * testData2[i])) - data[i]) > 1e-3) {
			FAIL();
		}
	}

	auto endOpenCL = std::chrono::high_resolution_clock::now();

	for (int i = 0; i < testSize; i++) {
		data[i] = sqrt(testData[i] + 2 * testData2[i]);
	}

	for (int i = 0; i < testSize; i++) {
		if (abs(sqrt((testData[i] + 2 * testData2[i])) - data[i]) > 1e-3) {
			FAIL();
		}
	}

	auto endCPU = std::chrono::high_resolution_clock::now();

	std::ofstream outputFile("../../../test_results/test_results_opencl_vs_cpu_benchmark.txt");
	outputFile << "Device name: " << gpuManager.getDeviceName() << std::endl;
	outputFile << "OpenCL: " << std::chrono::duration_cast<std::chrono::nanoseconds>(endOpenCL - beginOpenCL).count() << "ns" << std::endl;
	outputFile << "CPU:    " << std::chrono::duration_cast<std::chrono::nanoseconds>(endCPU - endOpenCL).count() << "ns" << std::endl;
	outputFile.close();
}

struct GPUFace {
	math::Vector4 edgePlaneVWNormal;
	math::Vector4 edgePlaneWUNormal;

	math::Vector4 normal;
	math::Vector4 p0;
};

struct GPULightRay {
	math::Vector4 rayDir;
	math::Vector4 rayOrigin;
};

TEST(OpenCLTests, OpenCLIntersectionTest) {
	// Load mesh
	ObjFileLoader teapotObj;
	bool result = teapotObj.readObjFile("../../../models/teapot.obj");

	Mesh mesh;
	mesh.loadObjFileData(&teapotObj);
	mesh.setFastIntersectEnabled(false);

	// Set up opencl device
	GPUManagerOpenCL gpuManager;
	gpuManager.initialize("../../../opencl_programs/mantaray.cl");

	int faceCount = mesh.getFaceCount();
	int unitCount = 65536;

	int rayCount = 128;

	GPUMemory *faceInput = gpuManager.createMemoryBuffer(sizeof(GPUFace) * unitCount, GPUMemory::READ_ONLY);
	GPUMemory *rayInput = gpuManager.createMemoryBuffer(sizeof(GPULightRay) * rayCount, GPUMemory::READ_ONLY);
	GPUMemory *outputDepths = gpuManager.createMemoryBuffer((rayCount / 32) * sizeof(int) * unitCount, GPUMemory::WRITE_ONLY);

	LightRay ray;
	ray.setDirection(math::loadVector(1.0, 0.0, 0.0));
	ray.setSource(math::loadVector(-10, 1.0, 0.0));

	GPUFace *faceData = new GPUFace[unitCount];
	GPULightRay *lightRay = new GPULightRay[rayCount];

	const PrecomputedValues *cache = mesh.getPrecomputedValues();

	for (int i = 0; i < faceCount; i++) {
		faceData[i].edgePlaneVWNormal = math::getVector4(cache[i].edgePlaneVW.normal);
		faceData[i].edgePlaneVWNormal.w = (float)cache[i].edgePlaneVW.d;
		faceData[i].edgePlaneWUNormal = math::getVector4(cache[i].edgePlaneWU.normal);
		faceData[i].edgePlaneWUNormal.w = (float)cache[i].edgePlaneWU.d;
		faceData[i].normal = math::getVector4(cache[i].normal);
		faceData[i].p0 = math::getVector4(cache[i].p0);
	}

	faceInput->write(faceData);

	GPUKernel *kernel = gpuManager.createKernel("mesh_intersection_test");

	int *output = new int[(rayCount / 32)*unitCount];

	auto beginOpenCL = std::chrono::high_resolution_clock::now();

	for (int i = 0; i < rayCount; i++) {
		lightRay[i].rayDir = math::getVector4(ray.getDirection());
		lightRay[i].rayOrigin = math::getVector4(ray.getSource());
	}

	kernel->setArgument(0, faceInput);
	kernel->setArgument(1, rayInput);
	kernel->setArgument(2, outputDepths);

	rayInput->write(lightRay);

	kernel->run(unitCount);

	outputDepths->read((void *)output);

	auto endOpenCL = std::chrono::high_resolution_clock::now();

	auto startCPU = std::chrono::high_resolution_clock::now();

	for (int i = 0; i < 128; i++) {
		IntersectionPoint p;
		mesh.detectIntersection(&ray, &p);
	}

	auto endCPU = std::chrono::high_resolution_clock::now();

	std::ofstream outputFile("../../../test_results/test_results_intersection_opencl_vs_cpu_benchmark.txt");
	outputFile << "Device name: " << gpuManager.getDeviceName() << std::endl;
	outputFile << "OpenCL: " << std::chrono::duration_cast<std::chrono::nanoseconds>(endOpenCL - beginOpenCL).count() << "ns" << std::endl;
	outputFile << "CPU:    " << std::chrono::duration_cast<std::chrono::nanoseconds>(endCPU - startCPU).count() << "ns" << std::endl;
	outputFile.close();
}
