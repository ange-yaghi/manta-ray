#include <pch.h>

#include <obj_file_loader.h>
#include <mesh.h>
#include <light_ray.h>
#include <intersection_point.h>

#include <chrono>
#include <fstream>

using namespace manta;

TEST(MeshIntersectionTests, MeshIntersectionPerformance) {
	ObjFileLoader teapotObj;
	bool result = teapotObj.readObjFile("../../../models/teapot.obj");

	Mesh mesh;
	mesh.initialize(teapotObj.getFaceCount(), teapotObj.getVertexCount());

	Face *tFaces = mesh.getFaces();
	SimpleVertex *tVerts = mesh.getVertices();

	for (unsigned int i = 0; i < mesh.getFaceCount(); i++) {
		ObjFace *face = teapotObj.getFace(i);
		tFaces[i].u = face->v1 - 1;
		tFaces[i].v = face->v2 - 1;
		tFaces[i].w = face->v3 - 1;
	}

	for (unsigned int i = 0; i < teapotObj.getVertexCount(); i++) {
		math::Vector3 *v = teapotObj.getVertex(i);
		tVerts[i].location = math::loadVector(*v);
	}
	mesh.precomputeValues();
	mesh.setFastIntersectEnabled(false);

	LightRay ray;
	ray.setDirection(math::loadVector(1.0, 0.0, 0.0));
	ray.setSource(math::loadVector(-10, 1.0, 0.0));

	auto begin = std::chrono::high_resolution_clock::now();

	IntersectionPoint p;
	for (int i = 0; i < 10000; i++) {
		mesh.detectIntersection(&ray, &p);
	}

	auto end = std::chrono::high_resolution_clock::now();

	std::ofstream outputFile("../../../test_results/test_results_mesh_intersection.txt");
	if (p.m_intersection) outputFile << "Intersection detected" << std::endl;
	outputFile << "Precomputed values struct size: " << sizeof(PrecomputedValues) << std::endl;
	outputFile << std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count() << "ns" << std::endl;
	outputFile.close();
}