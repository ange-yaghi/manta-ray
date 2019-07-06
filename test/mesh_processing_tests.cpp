#include <pch.h>

#include <obj_file_loader.h>
#include <mesh.h>
#include <light_ray.h>
#include <coarse_intersection.h>

#include <chrono>
#include <fstream>

using namespace manta;

TEST(MeshProcessingTests, NoQuadSanityTest) {
	ObjFileLoader singleTriangleObj;
	bool result = singleTriangleObj.loadObjFile("../../../test/geometry/single_triangle.obj");

	Mesh mesh;
	mesh.loadObjFileData(&singleTriangleObj);
	mesh.setFastIntersectEnabled(false);

	mesh.findQuads();

	int quadCount = mesh.getQuadFaceCount();

	EXPECT_EQ(quadCount, 0);

	singleTriangleObj.destroy();
	mesh.destroy();
}

TEST(MeshProcessingTests, CubeTest) {
	ObjFileLoader cubeObj;
	bool result = cubeObj.loadObjFile("../../../test/geometry/cube.obj");

	Mesh mesh;
	mesh.loadObjFileData(&cubeObj);
	mesh.setFastIntersectEnabled(false);

	mesh.findQuads();

	int quadCount = mesh.getQuadFaceCount();
	int triCount = mesh.getTriangleFaceCount();

	EXPECT_EQ(quadCount, 6);
	EXPECT_EQ(triCount, 0);

	cubeObj.destroy();
	mesh.destroy();
}

TEST(MeshProcessingTests, DoubleCubeTest) {
	ObjFileLoader cubeObj;
	bool result = cubeObj.loadObjFile("../../../test/geometry/two_cubes.obj");

	Mesh mesh;
	mesh.loadObjFileData(&cubeObj);
	mesh.setFastIntersectEnabled(false);

	mesh.findQuads();

	int quadCount = mesh.getQuadFaceCount();
	int triCount = mesh.getTriangleFaceCount();

	EXPECT_EQ(quadCount, 12);
	EXPECT_EQ(triCount, 0);

	cubeObj.destroy();
	mesh.destroy();
}

TEST(MeshProcessingTests, PlaneTest) {
	ObjFileLoader cubeObj;
	bool result = cubeObj.loadObjFile("../../../test/geometry/plane.obj");

	Mesh mesh;
	mesh.loadObjFileData(&cubeObj);
	mesh.setFastIntersectEnabled(false);

	mesh.findQuads();

	const QuadFace *f = mesh.getQuadFace(0);

	int quadCount = mesh.getQuadFaceCount();
	int triCount = mesh.getTriangleFaceCount();

	math::Vector v1, v2, v3, v4;
	v1 = *mesh.getVertex(f->u);
	v2 = *mesh.getVertex(f->v);
	v3 = *mesh.getVertex(f->w);
	v4 = *mesh.getVertex(f->r);

	EXPECT_EQ(quadCount, 1);
	EXPECT_EQ(triCount, 0);

	// Check for consistent handedness
	math::Vector n1 = math::cross(math::sub(v2, v1), math::sub(v3, v1));
	math::Vector n2 = math::cross(math::sub(v3, v4), math::sub(v2, v4));

	math::real d = math::getScalar(math::dot(n1, n2));
	EXPECT_GE(d, (math::real)0.0);

	// Check for consistency of edge function
	n1 = math::cross(math::sub(v1, v2), math::sub(v3, v2));
	n2 = math::cross(math::sub(v4, v2), math::sub(v3, v2));

	math::real d2 = math::getScalar(math::dot(n1, n2));
	EXPECT_LE(d2, (math::real)0.0);

	cubeObj.destroy();
	mesh.destroy();
}
