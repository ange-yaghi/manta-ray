#include <pch.h>

#include "utilities.h"

#include <light_ray.h>
#include <sphere_primitive.h>
#include <intersection_point.h>
#include <intersection_list.h>
#include <primitives.h>

#include <manta_math.h>

using namespace manta;

TEST(PrimitiveTests, SphereSanityCheck) {
	SpherePrimitive sphere;
	sphere.setRadius(10.0f);

	EXPECT_FLOAT_EQ((float)sphere.getRadius(), 10.0f);

	sphere.setId(1);

	EXPECT_EQ(sphere.getId(), 1);
}

TEST(PrimitiveTests, SphereIntersection) {
	SpherePrimitive sphere;
	sphere.setRadius(1.0f);
	sphere.setPosition(math::constants::Zero);

	LightRay ray;
	ray.setDirection(math::loadVector(-1.0f, 0.0f, 0.0f));
	ray.setSource(math::loadVector(10.0f, 0.0f, 0.0f));

	IntersectionList list;
	//const CoarseIntersection *c = sphere.coarseIntersection(&ray, &list, nullptr, nullptr, 1E-2, nullptr);

	//EXPECT_TRUE(c != nullptr);
	//EXPECT_TRUE(list.getIntersectionCount() == 1);

	list.destroy();
}

TEST(PrimitiveTests, SphereMiss) {
	SpherePrimitive sphere;
	sphere.setRadius(1.0f);
	sphere.setPosition(math::constants::Zero);

	LightRay ray;
	ray.setDirection(math::loadVector(-1.0f, 0.0f, 0.0f));
	ray.setSource(math::loadVector(10.0f, 2.0f, 0.0f));

	IntersectionList list;
	//const CoarseIntersection *c = sphere.coarseIntersection(&ray, &list, nullptr, nullptr, 1E-2, nullptr);

	//EXPECT_TRUE(c == nullptr);
	//EXPECT_TRUE(list.getIntersectionCount() == 0);
}

TEST(PrimitiveTests, SphereIntersectionInside) {
	SpherePrimitive sphere;
	sphere.setRadius(1.0f);
	sphere.setPosition(math::constants::Zero);

	LightRay ray;
	ray.setDirection(math::loadVector(-1.0f, 0.0f, 0.0f));
	ray.setSource(math::loadVector(0.0f, 0.0f, 0.0f));

	IntersectionList list;
	//const CoarseIntersection *c = sphere.coarseIntersection(&ray, &list, nullptr, nullptr, 1E-2, nullptr);

	//EXPECT_TRUE(c != nullptr);
	//EXPECT_TRUE(list.getIntersectionCount() == 1);

	list.destroy();
}

TEST(PrimitiveTests, AABBSanityCheck) {
	LightRay ray;
	ray.setDirection(math::loadVector(-1.0, 0.0, 0.0));
	ray.setSource(math::loadVector(0.0, 0.0, 0.0));
	ray.calculateTransformations();

	AABB aabb;
	aabb.maxPoint = math::loadVector(1.0, 1.0, 1.0);
	aabb.minPoint = math::loadVector(-1.0, -1.0, -1.0);

	math::real tmin, tmax;
	bool detected = aabb.rayIntersect(ray, &tmin, &tmax);

	EXPECT_EQ(tmin, 0.0);
	EXPECT_EQ(tmax, 1.0);
	EXPECT_TRUE(detected);
}

TEST(PrimitiveTests, AABBCheck1) {
	LightRay ray;
	ray.setDirection(math::loadVector(-1.0, 0.0, 0.0));
	ray.setSource(math::loadVector(2.0, 0.0, 0.0));
	ray.calculateTransformations();

	AABB aabb;
	aabb.maxPoint = math::loadVector(1.0, 1.0, 1.0);
	aabb.minPoint = math::loadVector(-1.0, -1.0, -1.0);

	math::real tmin, tmax;
	bool detected = aabb.rayIntersect(ray, &tmin, &tmax);

	EXPECT_EQ(tmin, 1.0);
	EXPECT_EQ(tmax, 3.0);
	EXPECT_TRUE(detected);
}

TEST(PrimitiveTests, AABBEdgeTest) {
	LightRay ray;
	ray.setDirection(math::loadVector(-1.0, 0.0, 0.0));
	ray.setSource(math::loadVector(2.0, 1.0, 1.0));
	ray.calculateTransformations();

	AABB aabb;
	aabb.maxPoint = math::loadVector(1.0, 1.0, 1.0);
	aabb.minPoint = math::loadVector(-1.0, -1.0, -1.0);

	math::real tmin, tmax;
	bool detected = aabb.rayIntersect(ray, &tmin, &tmax);

	EXPECT_EQ(tmin, 1.0);
	EXPECT_EQ(tmax, 3.0);
	EXPECT_TRUE(detected);
}

TEST(PrimitiveTests, AABBNoHitTest) {
	LightRay ray;
	ray.setDirection(math::loadVector(-1.0, 0.0, 0.0));
	ray.setSource(math::loadVector(2.0, 1.1, 1.0));
	ray.calculateTransformations();

	AABB aabb;
	aabb.maxPoint = math::loadVector(1.0, 1.0, 1.0);
	aabb.minPoint = math::loadVector(-1.0, -1.0, -1.0);

	math::real tmin, tmax;
	bool detected = aabb.rayIntersect(ray, &tmin, &tmax);

	EXPECT_FALSE(detected);
}
