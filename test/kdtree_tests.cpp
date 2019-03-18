#include <pch.h>

#include <obj_file_loader.h>
#include <mesh.h>
#include <light_ray.h>
#include <coarse_intersection.h>
#include <kd_tree.h>
#include <scene_object.h>

#include <chrono>
#include <fstream>

using namespace manta;

TEST(KDTreeTests, KDTreeSanityTest) {
	ObjFileLoader singleTriangleObj;
	bool result = singleTriangleObj.readObjFile("../../../test/geometry/single_triangle.obj");

	Mesh mesh;
	mesh.loadObjFileData(&singleTriangleObj);
	mesh.setFastIntersectEnabled(false);

	KDTree kdTree;
	kdTree.initialize(10, math::constants::Zero);

	LightRay ray;
	ray.setDirection(math::loadVector(0.0, 0.0, -1.0));
	ray.setSource(math::loadVector(1.0, 1.0, 1.0));
	ray.calculateTransformations();

	kdTree.analyze(&mesh, 16);

	singleTriangleObj.destroy();
	mesh.destroy();

	EXPECT_EQ(kdTree.getNode(0)->getSplitAxis(), 0x3);
	EXPECT_EQ(kdTree.getNode(0)->getObjectOffset(), 0x0);
	EXPECT_EQ(kdTree.getNode(0)->getPrimitiveCount(), 0x1);

	kdTree.destroy();
}

TEST(KDTreeTests, KDTreeIntersectionTest) {
	ObjFileLoader singleTriangleObj;
	bool result = singleTriangleObj.readObjFile("../../../test/geometry/single_triangle.obj");

	Mesh mesh;
	mesh.loadObjFileData(&singleTriangleObj);
	mesh.setFastIntersectEnabled(false);

	KDTree kdTree;
	kdTree.initialize(10, math::constants::Zero);

	LightRay ray;
	ray.setDirection(math::loadVector(0.0, 0.0, -1.0));
	ray.setSource(math::loadVector(0.5, 0.0, 1.0));
	ray.calculateTransformations();

	kdTree.analyze(&mesh, 16);
	kdTree.writeToObjFile("../../../workspace/test_results/kdtree_debug.obj");

	CoarseIntersection c;
	bool intersect = kdTree.findClosestIntersection(&ray, &c, (math::real)0.0, math::constants::REAL_MAX, nullptr /**/ STATISTICS_NULL_INPUT);

	singleTriangleObj.destroy();
	mesh.destroy();

	EXPECT_TRUE(intersect);
	EXPECT_FLOAT_EQ(c.depth, 1.0);

	kdTree.destroy();
}

TEST(KDTreeTests, KDTreeCubeTest) {
	ObjFileLoader singleTriangleObj;
	bool result = singleTriangleObj.readObjFile("../../../test/geometry/cube.obj");

	Mesh mesh;
	mesh.loadObjFileData(&singleTriangleObj);
	mesh.setFastIntersectEnabled(false);

	KDTree kdTree;
	kdTree.initialize(10, math::constants::Zero);

	LightRay ray;
	ray.setDirection(math::loadVector(0.0, 0.0, -1.0));
	ray.setSource(math::loadVector(0.5, 0.0, 2.0));
	ray.calculateTransformations();

	kdTree.analyze(&mesh, 16);
	kdTree.writeToObjFile("../../../workspace/test_results/kdtree_debug.obj");

	CoarseIntersection c;
	bool intersect = kdTree.findClosestIntersection(&ray, &c, (math::real)0.0, math::constants::REAL_MAX, nullptr /**/ STATISTICS_NULL_INPUT);

	singleTriangleObj.destroy();
	mesh.destroy();

	EXPECT_TRUE(intersect);
	EXPECT_NEAR(c.depth, 1.0, 1E-5);

	kdTree.destroy();
}

TEST(KDTreeTests, KDTreeDoubleCubeTest) {
	ObjFileLoader singleTriangleObj;
	bool result = singleTriangleObj.readObjFile("../../../test/geometry/two_cubes.obj");

	Mesh mesh;
	mesh.loadObjFileData(&singleTriangleObj);
	mesh.setFastIntersectEnabled(false);

	KDTree kdTree;
	kdTree.initialize(10, math::constants::Zero);

	LightRay ray1;
	ray1.setDirection(math::loadVector(0.0, -1.0, 0.0));
	ray1.setSource(math::loadVector(0.0, 2.0, 2.0));
	ray1.calculateTransformations();

	LightRay ray2;
	ray2.setDirection(math::loadVector(0.0, -1.0, 0.0));
	ray2.setSource(math::loadVector(0.0, 2.0, 0.0));
	ray2.calculateTransformations();

	LightRay ray3;
	ray3.setDirection(math::loadVector(0.0, -1.0, 0.0));
	ray3.setSource(math::loadVector(0.0, 2.0, -2.0));
	ray3.calculateTransformations();

	kdTree.analyze(&mesh, 4);
	kdTree.writeToObjFile("../../../workspace/test_results/kdtree_debug.obj");

	CoarseIntersection c;
	bool intersect = kdTree.findClosestIntersection(&ray1, &c, (math::real)0.0, math::constants::REAL_MAX, nullptr /**/ STATISTICS_NULL_INPUT);
	EXPECT_TRUE(intersect);
	EXPECT_NEAR(c.depth, 1.0, 1E-5);

	intersect = kdTree.findClosestIntersection(&ray2, &c, (math::real)0.0, math::constants::REAL_MAX, nullptr /**/ STATISTICS_NULL_INPUT);
	EXPECT_FALSE(intersect);

	intersect = kdTree.findClosestIntersection(&ray3, &c, (math::real)0.0, math::constants::REAL_MAX, nullptr /**/ STATISTICS_NULL_INPUT);
	EXPECT_TRUE(intersect);
	EXPECT_NEAR(c.depth, 1.0, 1E-5);

	singleTriangleObj.destroy();
	mesh.destroy();	

	kdTree.destroy();
}

TEST(KDTreeTests, KDTreeDoubleCubeSideTest) {
	ObjFileLoader singleTriangleObj;
	bool result = singleTriangleObj.readObjFile("../../../test/geometry/two_cubes.obj");

	Mesh mesh;
	mesh.loadObjFileData(&singleTriangleObj);
	mesh.setFastIntersectEnabled(false);

	KDTree kdTree;
	kdTree.initialize(10, math::constants::Zero);

	LightRay ray1;
	ray1.setDirection(math::loadVector(1.0, 0.0, 0.0));
	ray1.setSource(math::loadVector(-2.0, 0.0, 2.0));
	ray1.calculateTransformations();

	LightRay ray2;
	ray2.setDirection(math::loadVector(1.0, 0.0, 0.0));
	ray2.setSource(math::loadVector(-2.0, 0.0, 0.0));
	ray2.calculateTransformations();

	LightRay ray3;
	ray3.setDirection(math::loadVector(1.0, 0.0, 0.0));
	ray3.setSource(math::loadVector(-2.0, 0.0, -2.0));
	ray3.calculateTransformations();

	LightRay ray4;
	ray4.setDirection(math::loadVector(0.0, 0.0, -1.0));
	ray4.setSource(math::loadVector(0.0, 0.0, 4.0));
	ray4.calculateTransformations();

	kdTree.analyze(&mesh, 4);
	kdTree.writeToObjFile("../../../workspace/test_results/kdtree_debug.obj");

	CoarseIntersection c;
	bool intersect = kdTree.findClosestIntersection(&ray1, &c, (math::real)0.0, math::constants::REAL_MAX, nullptr /**/ STATISTICS_NULL_INPUT);
	EXPECT_TRUE(intersect);
	EXPECT_NEAR(c.depth, 1.0, 1E-5);

	intersect = kdTree.findClosestIntersection(&ray2, &c, (math::real)0.0, math::constants::REAL_MAX, nullptr /**/ STATISTICS_NULL_INPUT);
	EXPECT_FALSE(intersect);

	intersect = kdTree.findClosestIntersection(&ray3, &c, (math::real)0.0, math::constants::REAL_MAX, nullptr /**/ STATISTICS_NULL_INPUT);
	EXPECT_TRUE(intersect);
	EXPECT_NEAR(c.depth, 1.0, 1E-5);

	bool correctIntersect = mesh.findClosestIntersection(&ray4, &c, (math::real)0.0, math::constants::REAL_MAX, nullptr /**/ STATISTICS_NULL_INPUT);
	intersect = kdTree.findClosestIntersection(&ray4, &c, (math::real)0.0, math::constants::REAL_MAX, nullptr /**/ STATISTICS_NULL_INPUT);
	EXPECT_TRUE(intersect);
	EXPECT_NEAR(c.depth, 1.0, 1E-5);

	singleTriangleObj.destroy();
	mesh.destroy();

	kdTree.destroy();
}
