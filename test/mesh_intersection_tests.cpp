#include <pch.h>

#include <obj_file_loader.h>
#include <mesh.h>
#include <light_ray.h>
#include <coarse_intersection.h>

#include <chrono>
#include <fstream>

using namespace manta;

TEST(MeshIntersectionTests, MeshIntersectionSanityCheck) {
	ObjFileLoader singleTriangleObj;
	bool result = singleTriangleObj.readObjFile("../../../test/geometry/single_triangle.obj");

	Mesh mesh;
	mesh.loadObjFileData(&singleTriangleObj);
	mesh.setFastIntersectEnabled(false);

	LightRay ray;
	ray.setDirection(math::loadVector(0.0, 0.0, -1.0));
	ray.setSource(math::loadVector(0.5, 0.0, 1.0));
	ray.calculateTransformations();

	CoarseIntersection intersection;
	bool found = mesh.findClosestIntersection(&ray, &intersection, 0.0, math::constants::REAL_MAX, nullptr);

	EXPECT_EQ(found, true);
	EXPECT_FLOAT_EQ(intersection.depth, 1.0);

	singleTriangleObj.destroy();
	mesh.destroy();
}

TEST(MeshIntersectionTests, MeshIntersectionSanityCheckOppositeSide) {
	ObjFileLoader singleTriangleObj;
	bool result = singleTriangleObj.readObjFile("../../../test/geometry/single_triangle.obj");

	Mesh mesh;
	mesh.loadObjFileData(&singleTriangleObj);
	mesh.setFastIntersectEnabled(false);

	LightRay ray;
	ray.setDirection(math::loadVector(0.0, 0.0, 1.0));
	ray.setSource(math::loadVector(0.5, 0.0, -1.0));
	ray.calculateTransformations();

	CoarseIntersection intersection;
	bool found = mesh.findClosestIntersection(&ray, &intersection, 0.0, math::constants::REAL_MAX, nullptr);

	EXPECT_EQ(found, true);
	EXPECT_FLOAT_EQ(intersection.depth, 1.0);

	singleTriangleObj.destroy();
	mesh.destroy();
}

TEST(MeshIntersectionTests, MeshIntersectionSanityCheckNoHit) {
	ObjFileLoader singleTriangleObj;
	bool result = singleTriangleObj.readObjFile("../../../test/geometry/single_triangle.obj");

	Mesh mesh;
	mesh.loadObjFileData(&singleTriangleObj);
	mesh.setFastIntersectEnabled(false);

	LightRay ray;
	ray.setDirection(math::loadVector(0.0, 0.0, 1.0));
	ray.setSource(math::loadVector(5.5, 0.0, -1.0));
	ray.calculateTransformations();

	CoarseIntersection intersection;
	bool found = mesh.findClosestIntersection(&ray, &intersection, 0.0, math::constants::REAL_MAX, nullptr);

	EXPECT_EQ(found, false);

	singleTriangleObj.destroy();
	mesh.destroy();
}

TEST(MeshIntersectionTests, MeshIntersectionCheckVertex) {
	ObjFileLoader singleTriangleObj;
	bool result = singleTriangleObj.readObjFile("../../../test/geometry/single_triangle.obj");

	Mesh mesh;
	mesh.loadObjFileData(&singleTriangleObj);
	mesh.setFastIntersectEnabled(false);

	LightRay ray;
	ray.setDirection(math::loadVector(0.0, 0.0, -1.0));
	ray.setSource(math::loadVector(1.0, 1.0, 1.0));
	ray.calculateTransformations();

	CoarseIntersection intersection;
	bool found = mesh.findClosestIntersection(&ray, &intersection, 0.0, math::constants::REAL_MAX, nullptr);

	EXPECT_EQ(found, true);
	EXPECT_FLOAT_EQ(intersection.depth, 1.0);

	singleTriangleObj.destroy();
	mesh.destroy();
}

TEST(MeshIntersectionTests, MeshIntersectionSideIntersection) {
	ObjFileLoader singleTriangleObj;
	bool result = singleTriangleObj.readObjFile("../../../test/geometry/single_triangle.obj");

	Mesh mesh;
	mesh.loadObjFileData(&singleTriangleObj);
	mesh.setFastIntersectEnabled(false);

	LightRay ray;
	ray.setDirection(math::loadVector(1.0, 0.0, 0.0));
	ray.setSource(math::loadVector(1.0, 0.0, 0.0));
	ray.calculateTransformations();

	CoarseIntersection intersection;
	bool found = mesh.findClosestIntersection(&ray, &intersection, 0.0, math::constants::REAL_MAX, nullptr);

	EXPECT_EQ(found, false);

	singleTriangleObj.destroy();
	mesh.destroy();
}
