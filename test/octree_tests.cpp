#include <pch.h>

#include <obj_file_loader.h>
#include <mesh.h>
#include <light_ray.h>
#include <intersection_list.h>
#include <octree.h>
#include <scene_object.h>

#include <chrono>
#include <fstream>

using namespace manta;

TEST(OctreeTests, OctreeIntersectionTest) {
	Octree octree;
	octree.initialize(1.0, math::loadVector(100, 100, 100));

	bool test = octree.checkTriangle(
		math::loadVector(0.0, 100.0, 0.0),
		math::loadVector(100.0, 100.0, 100.0),
		math::loadVector(0.0, 100.0, 110.0)
	);

	int a = 0;
}

TEST(OctreeTests, OctreeTestSanityCheck) {
	ObjFileLoader teapotObj;
	bool result = teapotObj.readObjFile("../../../demos/models/teapot.obj");

	Mesh mesh;
	mesh.loadObjFileData(&teapotObj);
	mesh.setFastIntersectEnabled(false);
	
	Octree octree;
	octree.initialize(10.0, math::loadVector(0, 0, 0));
	octree.analyze(&mesh, 4);

	LightRay ray;
	ray.setDirection(math::normalize(math::loadVector(0.5, -1.0, 0.5)));
	ray.setSource(math::loadVector(0.5, 10, 0.5));

	octree.writeToObjFile("../../../workspace/test_results/octree_debug.obj", &ray);

	auto beginOctree = std::chrono::high_resolution_clock::now();

	IntersectionList list;
	for (int i = 0; i < 100000; i++) {
		octree.coarseIntersection(&ray, &list, nullptr, nullptr, 1E-2, nullptr);
		list.destroy();
	}

	auto endOctree = std::chrono::high_resolution_clock::now();

	auto begin = std::chrono::high_resolution_clock::now();

	IntersectionList list2;
	for (int i = 0; i < 100000; i++) {
		mesh.coarseIntersection(&ray, &list2, nullptr, nullptr, 1E-2, nullptr);
		list2.destroy();
	}

	auto end = std::chrono::high_resolution_clock::now();

	std::ofstream outputFile("../../../workspace/test_results/test_results_octree.txt");

	long long octreeTime = std::chrono::duration_cast<std::chrono::nanoseconds>(endOctree - beginOctree).count();
	long long standardTime = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count();

	outputFile << octreeTime << "ns" << std::endl;
	outputFile << standardTime << "ns" << std::endl;
	outputFile << (double)standardTime / octreeTime << "x" << std::endl;
	outputFile.close();

	octree.destroy();
	list.destroy();
	list2.destroy();
}
