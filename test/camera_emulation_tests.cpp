#include <pch.h>
#include "utilities.h"

#include <biconvex_lens.h>
#include <light_ray.h>

using namespace manta;

TEST(CameraEmulationTests, BiconvexLensSanityCheck) {
	manta::BiconvexLens lens;
	lens.setDepth(0.1);
	lens.setDirection(math::loadVector(1.0, 0.0, 0.0));
	lens.setInputSurfaceRadius(10.0);
	lens.setOutputSurfaceRadius(10.0);
	lens.setRadius(1.0);
	lens.setPosition(math::loadVector(0.0, 0.0, 0.0));
	lens.configure();

	LightRay ray1;
	ray1.setSource(math::loadVector(-10, 0.5, 0.0));
	ray1.setDirection(math::loadVector(1.0, 0.0, 0.0));

	LightRay ray2;
	ray2.setSource(math::loadVector(-10, 1.5, 0.0));
	ray2.setDirection(math::loadVector(1.0, 0.0, 0.0));

	LightRay transformed;
	bool flag;
	flag = lens.transformLightRay(&ray1, &transformed);

	EXPECT_TRUE(flag);
	EXPECT_LT(math::getY(transformed.getDirection()), 0.0);
	EXPECT_GT(math::getX(transformed.getSource()), 0.0);
	EXPECT_GT(math::getX(transformed.getDirection()), 0.0);

	flag = lens.transformLightRay(&ray2, &transformed);

	EXPECT_FALSE(flag);	
}

TEST(CameraEmulationTests, BiconvexLensConvergenceCheck) {
	manta::BiconvexLens lens;
	lens.setDepth(0.1);
	lens.setDirection(math::loadVector(1.0, 0.0, 0.0));
	lens.setInputSurfaceRadius(10.0);
	lens.setOutputSurfaceRadius(10.0);
	lens.setRadius(1.0);
	lens.setPosition(math::loadVector(0.0, 0.0, 0.0));
	lens.configure();

	LightRay ray1;
	ray1.setSource(math::loadVector(-10, 0.5, 0.0));
	ray1.setDirection(math::loadVector(1.0, 0.0, 0.0));

	LightRay transformed;
	bool flag;
	flag = lens.transformLightRay(&ray1, &transformed);

	math::real focalLength = lens.calculateFocalLength();

	math::Vector proj = math::dot(lens.getDirection(), transformed.getDirection());
	proj = math::mul(proj, math::loadScalar(focalLength));
	proj = math::mul(proj, transformed.getDirection());
	proj = math::add(proj, transformed.getSource());

	EXPECT_NEAR(math::getY(proj), 0.0, 0.01);
}
