#include <pch.h>

#include "utilities.h"

#include <light_ray.h>
#include <sphere_primitive.h>
#include <intersection_point.h>

#include <manta_math.h>

using namespace manta;

TEST(PrimitiveTests, SphereSanityCheck) {
	SpherePrimitive sphere;
	sphere.setRadius(10.0f);

	EXPECT_FLOAT_EQ(sphere.getRadius(), 10.0f);

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

	IntersectionPoint point;

	sphere.detectIntersection(&ray, &point);

	EXPECT_TRUE(point.m_intersection);

	CHECK_VEC(point.m_normal, 1.0f, 0.0f, 0.0f, 0.0f);
	CHECK_VEC(point.m_position, 1.0f, 0.0f, 0.0f, 0.0f);
}

TEST(PrimitiveTests, SphereMiss) {
	SpherePrimitive sphere;
	sphere.setRadius(1.0f);
	sphere.setPosition(math::constants::Zero);

	LightRay ray;
	ray.setDirection(math::loadVector(-1.0f, 0.0f, 0.0f));
	ray.setSource(math::loadVector(10.0f, 2.0f, 0.0f));

	IntersectionPoint point;

	sphere.detectIntersection(&ray, &point);

	EXPECT_FALSE(point.m_intersection);
}

TEST(PrimitiveTests, SphereIntersectionInside) {
	SpherePrimitive sphere;
	sphere.setRadius(1.0f);
	sphere.setPosition(math::constants::Zero);

	LightRay ray;
	ray.setDirection(math::loadVector(-1.0f, 0.0f, 0.0f));
	ray.setSource(math::loadVector(0.0f, 0.0f, 0.0f));

	IntersectionPoint point;

	sphere.detectIntersection(&ray, &point);

	EXPECT_TRUE(point.m_intersection);

	CHECK_VEC(point.m_normal, -1.0f, 0.0f, 0.0f, 0.0f);
	CHECK_VEC(point.m_position, -1.0f, 0.0f, 0.0f, 0.0f);
}