#include <pch.h>
#include "utilities.h"

#include <biconvex_lens.h>
#include <light_ray.h>
#include <simple_lens.h>

using namespace manta;

TEST(CameraEmulationTests, BiconvexLensSanityCheck) {
	manta::BiconvexLens lens;
	lens.setDepth(0.1f);
	lens.setDirection(math::loadVector(1.0f, 0.0f, 0.0f));
	lens.setInputSurfaceRadius(10.0f);
	lens.setOutputSurfaceRadius(10.0f);
	lens.setRadius(1.0f);
	lens.setPosition(math::loadVector(0.0f, 0.0f, 0.0f));
	lens.configure();

	LightRay ray1;
	ray1.setSource(math::loadVector(-10.f, 0.5f, 0.0f));
	ray1.setDirection(math::loadVector(1.0f, 0.0f, 0.0f));

	LightRay ray2;
	ray2.setSource(math::loadVector(-10.f, 1.5f, 0.0f));
	ray2.setDirection(math::loadVector(1.0f, 0.0f, 0.0f));

	LightRay transformed;
	bool flag;
	flag = lens.transformLightRay(&ray1, &transformed);

	EXPECT_TRUE(flag);
	EXPECT_LT(math::getY(transformed.getDirection()), 0.0f);
	EXPECT_GT(math::getX(transformed.getSource()), 0.0f);
	EXPECT_GT(math::getX(transformed.getDirection()), 0.0f);

	flag = lens.transformLightRay(&ray2, &transformed);

	EXPECT_FALSE(flag);	
}

TEST(CameraEmulationTests, BiconvexLensConvergenceCheck) {
	manta::BiconvexLens lens;
	lens.setDepth(0.1f);
	lens.setDirection(math::loadVector(1.0f, 0.0f, 0.0f));
	lens.setInputSurfaceRadius(10.0f);
	lens.setOutputSurfaceRadius(10.0f);
	lens.setRadius(1.0f);
	lens.setPosition(math::loadVector(0.0f, 0.0f, 0.0f));
	lens.configure();

	LightRay ray1;
	ray1.setSource(math::loadVector(-10.f, 0.5f, 0.0f));
	ray1.setDirection(math::loadVector(1.0f, 0.0f, 0.0f));

	LightRay transformed;
	bool flag;
	flag = lens.transformLightRay(&ray1, &transformed);

	math::real focalLength = lens.calculateFocalLength();

	math::Vector proj = math::dot(lens.getDirection(), transformed.getDirection());
	proj = math::mul(proj, math::loadScalar(focalLength));
	proj = math::mul(proj, transformed.getDirection());
	proj = math::add(proj, transformed.getSource());

	EXPECT_NEAR(math::getY(proj), 0.0f, 0.01f);
}

TEST(CameraEmulationTests, SimpleLensSanityCheck) {
	manta::SimpleLens lens;
	lens.initialize();
	lens.setPosition(math::loadVector(0.0f, 0.0f, 0.0f));
	lens.setDirection(math::loadVector(1.0f, 0.0f, 0.0f));
	lens.setUp(math::loadVector(0.0f, 1.0f, 0.0f));
	lens.setRadius(1.0f);
	lens.update();

	math::real lensHeight = 1.0f;
	math::real focusDistance = 50.0f;

	Aperture *aperture = lens.getAperture();
	aperture->setRadius((math::real)1.0);
	lens.setFocus(focusDistance);

	math::Vector sensorLocation = lens.getSensorLocation();

	LightRay ray1;
	ray1.setSource(sensorLocation);
	ray1.setDirection(math::normalize(math::loadVector(1.0f, 0.5f * lensHeight / -math::getX(sensorLocation), 0.0f)));

	LightRay transformed;
	bool flag;
	flag = lens.transformRay(&ray1, &transformed);

	EXPECT_TRUE(flag);

	math::Vector proj = math::div(math::loadScalar(focusDistance), math::dot(transformed.getDirection(), lens.getDirection()));
	proj = math::add(math::mul(proj, transformed.getDirection()), transformed.getSource());

	EXPECT_NEAR(math::getY(proj), 0.0f, 0.1f);
}

TEST(CameraEmulationTests, SimpleLensCacheTest) {
	manta::SimpleLens lens;
	lens.initialize();
	lens.setPosition(math::loadVector(0.0f, 0.0f, 0.0f));
	lens.setDirection(math::loadVector(1.0f, 0.0f, 0.0f));
	lens.setUp(math::loadVector(0.0f, 1.0f, 0.0f));
	lens.setRadius(1.0f);
	lens.update();

	math::real lensHeight = 1.0f;
	math::real focusDistance = 50.0f;

	Aperture *aperture = lens.getAperture();
	aperture->setRadius((math::real)0.05);
	lens.setFocus(focusDistance);

	math::Vector sensorLocation = lens.getSensorLocation();
	sensorLocation = math::add(sensorLocation, math::loadVector(0.0f, 10.0f, 0.0f));

	LightRay ray1;
	ray1.setSource(sensorLocation);
	ray1.setDirection(math::normalize(math::loadVector(1.0f, 0.5f * lensHeight / -math::getX(sensorLocation), 0.0f)));

	LensScanHint hint;
	lens.lensScan(sensorLocation, &hint, 4, 1.0f);

	int a = 0;
}
