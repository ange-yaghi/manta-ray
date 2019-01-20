#include <pch.h>

#include "utilities.h"

#include <light_ray.h>
#include <sphere_primitive.h>
#include <intersection_point.h>
#include <scene.h>
#include <scene_object.h>
#include <scene_geometry.h>
#include <material.h>
#include <ray_tracer.h>
#include <image_handling.h>
#include <memory_management.h>
#include <scene_buffer.h>
#include <standard_camera_ray_emitter_group.h>
#include <grid_sampler.h>

#include <manta_math.h>

using namespace manta;

TEST(IntegrationTests, BasicTest) {
	int resolutionX = 10;
	int resolutionY = 10;

	Scene scene;

	Material sphereMaterial;
	Material lightMaterial;

	sphereMaterial.setEmission(math::constants::Zero);
	lightMaterial.setEmission(math::loadVector(1.0f, 1.0f, 1.0f));

	SpherePrimitive sphere;
	sphere.setRadius(10.0f);
	sphere.setPosition(math::constants::Zero);

	SpherePrimitive light;
	light.setRadius(1.0f);
	light.setPosition(math::loadVector(15.0f, 15.0f, 15.0f));

	SceneObject *sphereObject = scene.createSceneObject();
	sphereObject->setDefaultMaterial(&sphereMaterial);
	sphereObject->setGeometry(&sphere);

	SceneObject *lightObject = scene.createSceneObject();
	lightObject->setDefaultMaterial(&lightMaterial);
	lightObject->setGeometry(&light);

	GridSampler sampler;
	sampler.setGridWidth(2);

	StandardCameraRayEmitterGroup cameraEmitter;
	cameraEmitter.setSampler(&sampler);
	cameraEmitter.setDirection(math::loadVector(-1.0f, 0.0f, 0.0f));
	cameraEmitter.setPosition(math::loadVector(10.0f, 0.0f, 0.0f));
	cameraEmitter.setUp(math::loadVector(0.0f, 1.0f, 0.0f));
	cameraEmitter.setPlaneDistance(1.0f);
	cameraEmitter.setPlaneHeight(1.0f);
	cameraEmitter.setResolutionX(resolutionX);
	cameraEmitter.setResolutionY(resolutionY);
	cameraEmitter.setSampleCount(1);

	RayTracer rayTracer;
	rayTracer.initialize(500 * MB, 100 * MB, 1, 1000, false);
	rayTracer.traceAll(&scene, &cameraEmitter);

	// Output the results to a scene buffer
	SceneBuffer sceneBuffer;
	cameraEmitter.fillSceneBuffer(&sceneBuffer);

	EXPECT_EQ(cameraEmitter.getEmitterCount(), 100);

	// Clean up the camera
	cameraEmitter.destroyRays();
	cameraEmitter.destroyEmitters();

	//SaveImageData(pixels, 10, 10, "test.bmp");

	sceneBuffer.destroy();
	rayTracer.destroy();
}
