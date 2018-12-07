#include <pch.h>

#include <obj_file_loader.h>
#include <mesh.h>
#include <light_ray.h>
#include <intersection_list.h>

#include <chrono>
#include <fstream>

using namespace manta;

TEST(MeshIntersectionTests, MeshIntersectionPerformance) {
	ObjFileLoader teapotObj;
	bool result = teapotObj.readObjFile("../../../demos/models/teapot.obj");

	Mesh mesh;
	mesh.loadObjFileData(&teapotObj);
	mesh.setFastIntersectEnabled(false);

	LightRay ray;
	ray.setDirection(math::loadVector(1.0, 0.0, 0.0));
	ray.setSource(math::loadVector(-10, 1.0, 0.0));

	auto begin = std::chrono::high_resolution_clock::now();

	IntersectionList list;
	for (int i = 0; i < 100; i++) {
		mesh.coarseIntersection(&ray, &list, nullptr, nullptr, 1E-2, nullptr);
	}

	auto end = std::chrono::high_resolution_clock::now();

	std::ofstream outputFile("../../../test_results/test_results_mesh_intersection.txt");
	if (list.getIntersectionCount() > 0) outputFile << "Intersection detected" << std::endl;
	outputFile << "Precomputed values struct size: " << sizeof(PrecomputedValues) << std::endl;
	outputFile << std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count() << "ns" << std::endl;
	outputFile.close();

	list.destroy();
}
