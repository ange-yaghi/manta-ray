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
#include <camera_ray_emitter.h>
#include <camera_ray_emitter_group.h>
#include <image_handling.h>

#include <manta_math.h>

using namespace manta;

TEST(IntegrationTests, BasicTest) {
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
	sphereObject->setMaterial(&sphereMaterial);
	sphereObject->setGeometry(&sphere);

	SceneObject *lightObject = scene.createSceneObject();
	lightObject->setMaterial(&lightMaterial);
	lightObject->setGeometry(&light);

	CameraRayEmitterGroup cameraEmitter;
	cameraEmitter.setSamplingWidth(2);
	cameraEmitter.setDirection(math::loadVector(-1.0f, 0.0f, 0.0f));
	cameraEmitter.setPosition(math::loadVector(10.0f, 0.0f, 0.0f));
	cameraEmitter.setUp(math::loadVector(0.0f, 1.0f, 0.0f));
	cameraEmitter.setPlaneDistance(1.0f);
	cameraEmitter.setPlaneHeight(1.0f);
	cameraEmitter.setResolutionX(10);
	cameraEmitter.setResolutionY(10);
	cameraEmitter.setSamplesPerPixel(1);

	RayTracer rayTracer;
	rayTracer.traceAll(&scene, &cameraEmitter);

	math::Vector *pixels = (math::Vector *)_aligned_malloc(sizeof(math::Vector) * 100, 16);
	
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			pixels[i * 10 + j] = ((CameraRayEmitter *)(cameraEmitter.getEmitters()[i * 10 + j]))->getIntensity();
		}
	}

	//SaveImageData(pixels, 10, 10, "test.bmp");

	EXPECT_EQ(cameraEmitter.getEmitterCount(), 100);
}
