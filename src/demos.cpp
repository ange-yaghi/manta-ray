#include <demos.h>

#include <light_ray.h>
#include <sphere_primitive.h>
#include <intersection_point.h>
#include <scene.h>
#include <scene_object.h>
#include <scene_geometry.h>
#include <material.h>
#include <simple_diffuse_material.h>
#include <simple_specular_diffuse_material.h>
#include <ray_tracer.h>
#include <camera_ray_emitter_group.h>
#include <camera_ray_emitter.h>
#include <image_handling.h>
#include <memory_management.h>
#include <mesh.h>
#include <obj_file_loader.h>
#include <manta_math.h>

#include <iostream>

manta::math::Vector getColor(int r, int g, int b) {
	return manta::math::loadVector(r / 255.0f, g / 255.0f, b / 255.0f);
}

void manta::simpleRoomDemo(int samplesPerPixel, int resolutionX, int resolutionY) {
	Scene scene;

	// Load all object files
	ObjFileLoader smallHouseObj;
	bool result = smallHouseObj.readObjFile("../../models/small_house_unsealed.obj");

	ObjFileLoader tableObj;
	result = tableObj.readObjFile("../../models/table.obj");

	ObjFileLoader shutterObj;
	result = shutterObj.readObjFile("../../models/room_shutters.obj");

	// Create all materials
	SimpleSpecularDiffuseMaterial wallMaterial;
	wallMaterial.setEmission(math::constants::Zero);
	wallMaterial.setDiffuseColor(getColor(200, 200, 200));
	wallMaterial.setSpecularColor(math::constants::Zero);

	SimpleSpecularDiffuseMaterial outdoorLight;
	outdoorLight.setEmission(math::loadVector(18, 16, 16));
	outdoorLight.setDiffuseColor(math::constants::Zero);
	outdoorLight.setSpecularColor(math::constants::Zero);

	SimpleSpecularDiffuseMaterial outdoorTopLightMaterial;
	outdoorTopLightMaterial.setEmission(math::loadVector(20, 20, 22));
	outdoorTopLightMaterial.setDiffuseColor(math::constants::Zero);
	outdoorTopLightMaterial.setSpecularColor(math::constants::Zero);

	SimpleSpecularDiffuseMaterial tableMaterial;
	tableMaterial.setEmission(math::constants::Zero);
	tableMaterial.setDiffuseColor(getColor(78, 46, 40));
	tableMaterial.setSpecularColor(getColor(100, 100, 100));

	SimpleSpecularDiffuseMaterial groundMaterial;
	groundMaterial.setEmission(math::constants::Zero);
	groundMaterial.setDiffuseColor(math::mul(math::loadVector(78, 46, 40), math::loadScalar(0.001)));
	groundMaterial.setSpecularColor(math::constants::Zero);

	// Create all scene geometry
	Mesh smallHouse;
	smallHouse.loadObjFileData(&smallHouseObj);
	smallHouse.setFastIntersectEnabled(false);
	smallHouse.setFastIntersectRadius((math::real)4.0);

	Mesh table;
	table.loadObjFileData(&tableObj);
	table.setFastIntersectEnabled(false);
	table.setFastIntersectRadius((math::real)4.0);

	Mesh shutters;
	shutters.loadObjFileData(&shutterObj);
	shutters.setFastIntersectEnabled(false);
	shutters.setFastIntersectRadius((math::real)4.0);

	SpherePrimitive outdoorLightGeometry;
	outdoorLightGeometry.setRadius((math::real)10.0);
	outdoorLightGeometry.setPosition(math::loadVector(10.5, 0.0, -20.5));

	SpherePrimitive outdoorTopLightGeometry;
	outdoorTopLightGeometry.setRadius((math::real)10.0);
	outdoorTopLightGeometry.setPosition(math::loadVector(0.0, 25.0, 2));
	//outdoorTopLightGeometry.setPosition(math::loadVector(0.0, 4.0, 0));
	//outdoorTopLightGeometry.setRadius((math::real)0.5);

	SpherePrimitive groundGeometry;
	groundGeometry.setRadius((math::real)50000.0);
	groundGeometry.setPosition(math::loadVector(0.0, -50000.1, 0));

	// Create scene objects
	SceneObject *smallHouseObject = scene.createSceneObject();
	smallHouseObject->setGeometry(&smallHouse);
	smallHouseObject->setMaterial(&wallMaterial);
	smallHouseObject->setName("House");

	SceneObject *tableObject = scene.createSceneObject();
	tableObject->setGeometry(&table);
	tableObject->setMaterial(&tableMaterial);
	tableObject->setName("Table");

	SceneObject *shuttersObject = scene.createSceneObject();
	shuttersObject->setGeometry(&shutters);
	shuttersObject->setMaterial(&wallMaterial);
	shuttersObject->setName("Shutters");

	//SceneObject *ground = scene.createSceneObject();
	//ground->setGeometry(&groundGeometry);
	//ground->setMaterial(&groundMaterial);

	//SceneObject *outdoorTopLightObject = scene.createSceneObject();
	//outdoorTopLightObject->setGeometry(&outdoorTopLightGeometry);
	//outdoorTopLightObject->setMaterial(&outdoorTopLightMaterial);

	//SceneObject *lightSource = scene.createSceneObject();
	//lightSource->setGeometry(&outdoorLightGeometry);
	//lightSource->setMaterial(&outdoorLight);

	// Create the camera
	CameraRayEmitterGroup camera;
	camera.setSamplingWidth(1);
	camera.setDirection(math::loadVector(-1.0, 0.0, 0.0));
	camera.setPosition(math::loadVector(5.0, 2.0, 0.0));
	camera.setUp(math::loadVector(0.0f, 1.0, 0.0));
	camera.setPlaneDistance(1.0f);
	camera.setPlaneHeight(1.0f);
	camera.setResolutionX(resolutionX);
	camera.setResolutionY(resolutionY);
	camera.setSamplesPerPixel(samplesPerPixel);
	
	// Create the raytracer
	RayTracer rayTracer;
	rayTracer.initialize(1000 * MB, 100 * MB, 12, 10000, true);
	rayTracer.setBackgroundColor(getColor(135, 206, 235));
	rayTracer.setDeterministicSeedMode(true);
	rayTracer.traceAll(&scene, &camera);
	// Leaks
	//rayTracer.tracePixel(518, 101, &scene, &camera);
	//rayTracer.tracePixel(495, 122, &scene, &camera);
	//rayTracer.tracePixel(389, 188, &scene, &camera);
	//rayTracer.tracePixel(1441, 227, &scene, &camera);
	//rayTracer.tracePixel(459, 358, &scene, &camera);
	//rayTracer.tracePixel(1160, 566, &scene, &camera);
	//rayTracer.tracePixel(2020, 739, &scene, &camera);
	//rayTracer.tracePixel(1094, 910, &scene, &camera);
	//rayTracer.tracePixel(1829, 1402, &scene, &camera);
	//rayTracer.tracePixel(839, 1417, &scene, &camera);
	//rayTracer.tracePixel(2026, 1443, &scene, &camera);
	//rayTracer.tracePixel(1215, 1511, &scene, &camera);

	// Output the results to file
	math::Vector *pixels = (math::Vector *)_aligned_malloc(sizeof(math::Vector) * resolutionX * resolutionY, 16);

	for (int i = 0; i < resolutionY; i++) {
		for (int j = 0; j < resolutionX; j++) {
			math::Vector v = math::constants::Zero;
			
			if (camera.getEmitters()[i * resolutionX + j] != nullptr) {
				v = ((CameraRayEmitter *)(camera.getEmitters()[i * resolutionX + j]))->getIntensity();
				math::real r = math::getX(v);
				math::real g = math::getY(v);
				math::real b = math::getZ(v);

				if (r > 0.0 || g > 0.0 || b > 0.0) {
					std::cout << "LEAK AT: " << j << ", " << i << std::endl;
				}
			}

			pixels[i * resolutionX + j] = v;
		}
	}

	// Clean everything up
	for (int i = camera.getEmitterCount() - 1; i >= 0; i--) {
		if (camera.getEmitters()[i] != nullptr) {
			((CameraRayEmitter *)(camera.getEmitters()[i]))->destroyRays();
		}
	}

	SaveImageData(pixels, resolutionX, resolutionY, "test.bmp");
	camera.destroyEmitters();

	rayTracer.destroy();
}

void manta::teapotDemo(int samplesPerPixel, int resolutionX, int resolutionY) {
	Scene scene;

	// Load all object files
	ObjFileLoader smallHouseObj;
	bool result = smallHouseObj.readObjFile("../../models/small_house.obj");

	ObjFileLoader teapotObj;
	result = teapotObj.readObjFile("../../models/teapot.obj");

	// Create all materials
	SimpleSpecularDiffuseMaterial wallMaterial;
	wallMaterial.setEmission(math::constants::Zero);
	wallMaterial.setDiffuseColor(getColor(200, 200, 200));
	wallMaterial.setSpecularColor(math::constants::Zero);

	SimpleSpecularDiffuseMaterial outdoorLight;
	outdoorLight.setEmission(math::loadVector(9, 8, 8));
	outdoorLight.setDiffuseColor(math::constants::Zero);
	outdoorLight.setSpecularColor(math::constants::Zero);

	SimpleSpecularDiffuseMaterial outdoorTopLightMaterial;
	outdoorTopLightMaterial.setEmission(math::loadVector(10, 10, 11));
	outdoorTopLightMaterial.setDiffuseColor(math::constants::Zero);
	outdoorTopLightMaterial.setSpecularColor(math::constants::Zero);

	SimpleSpecularDiffuseMaterial teapotMaterial;
	teapotMaterial.setEmission(math::constants::Zero);
	teapotMaterial.setDiffuseColor(getColor(1, 1, 1));
	teapotMaterial.setSpecularColor(getColor(100, 100, 100));

	SimpleSpecularDiffuseMaterial groundMaterial;
	groundMaterial.setEmission(math::constants::Zero);
	groundMaterial.setDiffuseColor(math::mul(math::loadVector(78, 46, 40), math::loadScalar(0.001)));
	groundMaterial.setSpecularColor(math::constants::Zero);

	// Create all scene geometry
	Mesh smallHouse;
	smallHouse.loadObjFileData(&smallHouseObj);
	smallHouse.setFastIntersectEnabled(false);
	smallHouse.setFastIntersectRadius((math::real)4.0);

	Mesh teapot;
	teapot.loadObjFileData(&teapotObj);
	teapot.setFastIntersectEnabled(false);
	teapot.setFastIntersectRadius((math::real)4.0);

	SpherePrimitive outdoorLightGeometry;
	outdoorLightGeometry.setRadius((math::real)10.0);
	outdoorLightGeometry.setPosition(math::loadVector(10.5, 0.0, -20.5));

	SpherePrimitive outdoorTopLightGeometry;
	outdoorTopLightGeometry.setRadius((math::real)10.0);
	outdoorTopLightGeometry.setPosition(math::loadVector(0.0, 25.0, 2));

	SpherePrimitive groundGeometry;
	groundGeometry.setRadius((math::real)50000.0);
	groundGeometry.setPosition(math::loadVector(0.0, -50000.1, 0));

	// Create scene objects
	//SceneObject *smallHouseObject = scene.createSceneObject();
	//smallHouseObject->setGeometry(&smallHouse);
	//smallHouseObject->setMaterial(&wallMaterial);

	SceneObject *teapotObject = scene.createSceneObject();
	teapotObject->setGeometry(&teapot);
	teapotObject->setMaterial(&teapotMaterial);

	//SceneObject *ground = scene.createSceneObject();
	//ground->setGeometry(&groundGeometry);
	//ground->setMaterial(&groundMaterial);

	//SceneObject *outdoorTopLightObject = scene.createSceneObject();
	//outdoorTopLightObject->setGeometry(&outdoorTopLightGeometry);
	//outdoorTopLightObject->setMaterial(&outdoorTopLightMaterial);

	//SceneObject *lightSource = scene.createSceneObject();
	//lightSource->setGeometry(&outdoorLightGeometry);
	//lightSource->setMaterial(&outdoorLight);

	// Create the camera
	CameraRayEmitterGroup camera;
	camera.setSamplingWidth(1);
	camera.setDirection(math::loadVector(-1.0, 0.0, 0.0));
	camera.setPosition(math::loadVector(5.0, 2.0, 0.0));
	camera.setUp(math::loadVector(0.0f, 1.0, 0.0));
	camera.setPlaneDistance(1.0f);
	camera.setPlaneHeight(1.0f);
	camera.setResolutionX(resolutionX);
	camera.setResolutionY(resolutionY);
	camera.setSamplesPerPixel(samplesPerPixel);

	// Create the raytracer
	RayTracer rayTracer;
	rayTracer.initialize(500 * MB, 50 * MB, 12, 10000, true);
	rayTracer.setBackgroundColor(getColor(135, 206, 235));
	rayTracer.setDeterministicSeedMode(true);
	//rayTracer.traceAll(&scene, &camera);
	rayTracer.tracePixel(817, 689, &scene, &camera);

	// Output the results to file
	math::Vector *pixels = (math::Vector *)_aligned_malloc(sizeof(math::Vector) * resolutionX * resolutionY, 16);

	for (int i = 0; i < resolutionY; i++) {
		for (int j = 0; j < resolutionX; j++) {
			math::Vector v = ((CameraRayEmitter *)(camera.getEmitters()[i * resolutionX + j]))->getIntensity();
			math::real r = math::getX(v);
			math::real g = math::getY(v);
			math::real b = math::getZ(v);

			pixels[i * resolutionX + j] = v;
		}
	}

	// Clean everything up
	for (int i = camera.getEmitterCount() - 1; i >= 0; i--) {
		((CameraRayEmitter *)(camera.getEmitters()[i]))->destroyRays();
	}

	SaveImageData(pixels, resolutionX, resolutionY, "test.bmp");
	camera.destroyEmitters();

	rayTracer.destroy();
}

void manta::lampDemo(int samplesPerPixel, int resolutionX, int resolutionY) {
	Scene scene;

	// Load all object files
	ObjFileLoader lampObj;
	bool result = lampObj.readObjFile("../../models/lamp2.obj");

	ObjFileLoader teapotObj;
	result = teapotObj.readObjFile("../../models/lamp_teapot.obj");

	ObjFileLoader lightBulbObj;
	result = lightBulbObj.readObjFile("../../models/lamp_lightbulb.obj");

	ObjFileLoader lampBlockObj;
	result = lampBlockObj.readObjFile("../../models/lampblock.obj");

	// Create all materials
	SimpleSpecularDiffuseMaterial wallMaterial;
	wallMaterial.setEmission(math::constants::Zero);
	wallMaterial.setDiffuseColor(getColor(200, 200, 200));
	wallMaterial.setSpecularColor(math::constants::Zero);

	SimpleSpecularDiffuseMaterial lampLightMaterial;
	lampLightMaterial.setEmission(math::mul(getColor(255, 197, 143), math::loadScalar(30.0)));
	lampLightMaterial.setDiffuseColor(math::constants::Zero);
	lampLightMaterial.setSpecularColor(math::constants::Zero);

	SimpleSpecularDiffuseMaterial teapotMaterial;
	teapotMaterial.setEmission(math::constants::Zero);
	teapotMaterial.setDiffuseColor(getColor(0xFF, 0x08, 0x14));
	teapotMaterial.setSpecularColor(getColor(100, 100, 100));

	SimpleSpecularDiffuseMaterial groundMaterial;
	groundMaterial.setEmission(math::constants::Zero);
	groundMaterial.setDiffuseColor(math::constants::Zero);
	groundMaterial.setSpecularColor(getColor(100, 100, 100));

	// Create all scene geometry
	Mesh lamp;
	lamp.loadObjFileData(&lampObj);
	lamp.setFastIntersectEnabled(true);
	lamp.setFastIntersectRadius((math::real)2.123);
	lamp.setFastIntersectPosition(math::loadVector(-0.06430, 1.86833, -2.96564));

	Mesh lampBlock;
	lampBlock.loadObjFileData(&lampBlockObj);
	lampBlock.setFastIntersectEnabled(false);
	lampBlock.setFastIntersectRadius((math::real)4.0);

	Mesh teapot;
	teapot.loadObjFileData(&teapotObj);
	teapot.setFastIntersectEnabled(true);
	teapot.setFastIntersectRadius((math::real)2.0);
	teapot.setFastIntersectPosition(math::loadVector(-0.5724, 1.02483, -0.04969));

	SpherePrimitive bulb;
	bulb.setRadius(0.25);
	bulb.setPosition(math::loadVector(0.10669, 3.42135, -2.47464));

	SpherePrimitive outdoorLightGeometry;
	outdoorLightGeometry.setRadius((math::real)10.0);
	outdoorLightGeometry.setPosition(math::loadVector(10.5, 0.0, -20.5));

	SpherePrimitive outdoorTopLightGeometry;
	outdoorTopLightGeometry.setRadius((math::real)10.0);
	outdoorTopLightGeometry.setPosition(math::loadVector(0.0, 25.0, 2));

	SpherePrimitive groundGeometry;
	groundGeometry.setRadius((math::real)50000.0);
	groundGeometry.setPosition(math::loadVector(0.0, -50000.001, 0));

	// Create scene objects
	SceneObject *lampObject = scene.createSceneObject();
	lampObject->setGeometry(&lamp);
	lampObject->setMaterial(&wallMaterial);
	lampObject->setName("Lamp");

	SceneObject *bulbObject = scene.createSceneObject();
	bulbObject->setGeometry(&bulb);
	bulbObject->setMaterial(&lampLightMaterial);
	bulbObject->setName("Bulb");

	SceneObject *teapotObject = scene.createSceneObject();
	teapotObject->setGeometry(&teapot);
	teapotObject->setMaterial(&teapotMaterial);
	teapotObject->setName("Teapot");

	SceneObject *ground = scene.createSceneObject();
	ground->setGeometry(&groundGeometry);
	ground->setMaterial(&wallMaterial);
	ground->setName("Ground");

	SceneObject *lampBlockObject = scene.createSceneObject();
	lampBlockObject->setGeometry(&lampBlock);
	lampBlockObject->setMaterial(&wallMaterial);
	lampBlockObject->setName("LampBlock");

	// Create the camera
	CameraRayEmitterGroup camera;
	camera.setSamplingWidth(1);
	camera.setDirection(math::loadVector(-1.0, 0.0, 0.0));
	camera.setPosition(math::loadVector(7.0, 2.0, 0.0));
	camera.setUp(math::loadVector(0.0f, 1.0, 0.0));
	camera.setPlaneDistance(1.0f);
	camera.setPlaneHeight(1.0f);
	camera.setResolutionX(resolutionX);
	camera.setResolutionY(resolutionY);
	camera.setSamplesPerPixel(samplesPerPixel);

	// Create the raytracer
	RayTracer rayTracer;
	rayTracer.initialize(1000 * MB, 100 * MB, 12, 10000, true);
	rayTracer.setBackgroundColor(getColor(0, 0, 0));
	rayTracer.setDeterministicSeedMode(true);
	//rayTracer.tracePixel(819, 199, &scene, &camera);
	//rayTracer.tracePixel(702, 236, &scene, &camera);
	//rayTracer.tracePixel(809, 211, &scene, &camera);
	//rayTracer.tracePixel(793, 224, &scene, &camera);
	//rayTracer.tracePixel(656, 185, &scene, &camera);
	//rayTracer.tracePixel(769, 318, &scene, &camera);
	//rayTracer.tracePixel(742, 218, &scene, &camera);
	//rayTracer.tracePixel(736, 331, &scene, &camera);

	// Leaks
	//rayTracer.tracePixel(1281, 900, &scene, &camera);
	//rayTracer.tracePixel(1456, 1230, &scene, &camera);
	//rayTracer.tracePixel(616, 1459, &scene, &camera);

	rayTracer.traceAll(&scene, &camera);

	// Output the results to file
	math::Vector *pixels = (math::Vector *)_aligned_malloc(sizeof(math::Vector) * resolutionX * resolutionY, 16);

	for (int i = 0; i < resolutionY; i++) {
		for (int j = 0; j < resolutionX; j++) {
			math::Vector v = ((CameraRayEmitter *)(camera.getEmitters()[i * resolutionX + j]))->getIntensity();
			math::real r = math::getX(v);
			math::real g = math::getY(v);
			math::real b = math::getZ(v);

			if (r > 0) {
				std::cout << i << ", " << j << std::endl;
			}

			pixels[i * resolutionX + j] = v;
		}
	}

	// Clean everything up
	for (int i = camera.getEmitterCount() - 1; i >= 0; i--) {
		((CameraRayEmitter *)(camera.getEmitters()[i]))->destroyRays();
	}

	SaveImageData(pixels, resolutionX, resolutionY, "test.bmp");
	camera.destroyEmitters();

	rayTracer.destroy();
}

void manta::cubeTestDemo(int samplesPerPixel, int resolutionX, int resolutionY) {
	Scene scene;

	// Load all object files
	ObjFileLoader cubeObj;
	bool result = cubeObj.readObjFile("../../models/cube.obj");

	// Create all materials
	SimpleSpecularDiffuseMaterial wallMaterial;
	wallMaterial.setEmission(math::constants::Zero);
	wallMaterial.setDiffuseColor(getColor(200, 200, 200));
	wallMaterial.setSpecularColor(math::constants::Zero);

	SimpleSpecularDiffuseMaterial outdoorLight;
	outdoorLight.setEmission(math::loadVector(9, 8, 8));
	outdoorLight.setDiffuseColor(math::constants::Zero);
	outdoorLight.setSpecularColor(math::constants::Zero);

	SimpleSpecularDiffuseMaterial outdoorTopLightMaterial;
	outdoorTopLightMaterial.setEmission(math::loadVector(10, 10, 11));
	outdoorTopLightMaterial.setDiffuseColor(math::constants::Zero);
	outdoorTopLightMaterial.setSpecularColor(math::constants::Zero);

	SimpleSpecularDiffuseMaterial teapotMaterial;
	teapotMaterial.setEmission(math::constants::Zero);
	teapotMaterial.setDiffuseColor(getColor(150, 0, 0));
	teapotMaterial.setSpecularColor(getColor(100, 100, 100));

	SimpleSpecularDiffuseMaterial groundMaterial;
	groundMaterial.setEmission(math::constants::Zero);
	groundMaterial.setDiffuseColor(math::constants::Zero);
	groundMaterial.setSpecularColor(getColor(100, 100, 100));

	// Create all scene geometry
	Mesh cube;
	cube.loadObjFileData(&cubeObj);
	cube.setFastIntersectEnabled(false);
	cube.setFastIntersectRadius((math::real)4.0);

	// Create scene objects
	SceneObject *cubeObject = scene.createSceneObject();
	cubeObject->setGeometry(&cube);
	cubeObject->setMaterial(&wallMaterial);

	//SceneObject *outdoorTopLightObject = scene.createSceneObject();
	//outdoorTopLightObject->setGeometry(&outdoorTopLightGeometry);
	//outdoorTopLightObject->setMaterial(&outdoorTopLightMaterial);

	//SceneObject *lightSource = scene.createSceneObject();
	//lightSource->setGeometry(&outdoorLightGeometry);
	//lightSource->setMaterial(&outdoorLight);

	// Create the camera
	CameraRayEmitterGroup camera;
	camera.setSamplingWidth(1);
	camera.setDirection(math::loadVector(-1.0, 0.0, 0.0));
	camera.setPosition(math::loadVector(7.0, 2.0, 0.0));
	camera.setUp(math::loadVector(0.0f, 1.0, 0.0));
	camera.setPlaneDistance(1.0f);
	camera.setPlaneHeight(1.0f);
	camera.setResolutionX(resolutionX);
	camera.setResolutionY(resolutionY);
	camera.setSamplesPerPixel(samplesPerPixel);

	// Create the raytracer
	RayTracer rayTracer;
	rayTracer.initialize(500 * MB, 500 * MB, 12, 10000, true);
	rayTracer.setBackgroundColor(getColor(0, 0, 0));
	rayTracer.traceAll(&scene, &camera);

	// Output the results to file
	math::Vector *pixels = (math::Vector *)_aligned_malloc(sizeof(math::Vector) * resolutionX * resolutionY, 16);

	for (int i = 0; i < resolutionY; i++) {
		for (int j = 0; j < resolutionX; j++) {
			math::Vector v = ((CameraRayEmitter *)(camera.getEmitters()[i * resolutionX + j]))->getIntensity();
			math::real r = math::getX(v);
			math::real g = math::getY(v);
			math::real b = math::getZ(v);

			pixels[i * resolutionX + j] = v;
		}
	}

	// Clean everything up
	for (int i = camera.getEmitterCount() - 1; i >= 0; i--) {
		((CameraRayEmitter *)(camera.getEmitters()[i]))->destroyRays();
	}

	SaveImageData(pixels, resolutionX, resolutionY, "cube_test.bmp");
	camera.destroyEmitters();

	rayTracer.destroy();
}
