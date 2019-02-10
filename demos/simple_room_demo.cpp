#include <demos.h>

#include <iostream>

using namespace manta;

void manta_demo::simpleRoomDemo(int samplesPerPixel, int resolutionX, int resolutionY) {
	// Top-level parameters
	constexpr bool BLOCK_ALL_LIGHT = false;
	constexpr bool USE_ACCELERATION_STRUCTURE = true;
	constexpr bool DETERMINISTIC_SEED_MODE = false;
	constexpr bool TRACE_SINGLE_PIXEL = false;
	constexpr bool WRITE_KDTREE_TO_FILE = false;

	Scene scene;
	RayTracer rayTracer;

	// Load all object files
	ObjFileLoader smallHouseObj;
	bool result = smallHouseObj.readObjFile(MODEL_PATH "small_house.obj");

	ObjFileLoader shutterObj;
	result &= shutterObj.readObjFile(MODEL_PATH "room_shutters.obj");

	if (!result) {
		std::cout << "Could not open geometry file" << std::endl;

		smallHouseObj.destroy();
		shutterObj.destroy();

		return;
	}

	// Create all materials
	LambertianBSDF lambert;
	
	SimpleBSDFMaterial *wallMaterial = rayTracer.getMaterialManager()->newMaterial<SimpleBSDFMaterial>();
	wallMaterial->setName("WallMaterial");
	wallMaterial->setBSDF(&lambert);
	wallMaterial->setReflectance(getColor(200, 200, 200));

	SimpleBSDFMaterial outdoorLight;
	outdoorLight.setEmission(math::loadVector(18, 16, 16));
	outdoorLight.setReflectance(math::constants::Zero);

	SimpleBSDFMaterial outdoorTopLightMaterial;
	outdoorTopLightMaterial.setEmission(math::loadVector(20, 20, 22));
	outdoorTopLightMaterial.setReflectance(math::constants::Zero);

	SimpleBSDFMaterial *tableMaterial = rayTracer.getMaterialManager()->newMaterial<SimpleBSDFMaterial>();
	tableMaterial->setName("TableMaterial");
	tableMaterial->setReflectance(getColor(78, 46, 40));
	tableMaterial->setBSDF(&lambert);

	SimpleBSDFMaterial *groundMaterial = rayTracer.getMaterialManager()->newMaterial<SimpleBSDFMaterial>();
	groundMaterial->setName("GroundMaterial");
	groundMaterial->setReflectance(math::mul(math::loadVector(78, 46, 40), math::loadScalar(0.001)));
	groundMaterial->setBSDF(&lambert);

	// Create all scene geometry
	Mesh smallHouse;
	smallHouse.loadObjFileData(&smallHouseObj, rayTracer.getMaterialManager(), -1, 0);
	smallHouse.setFastIntersectEnabled(false);

	Mesh shutters;
	shutters.loadObjFileData(&shutterObj);
	shutters.setFastIntersectEnabled(false);

	if (BLOCK_ALL_LIGHT) {
		smallHouse.merge(&shutters);
	}

	KDTree kdtree;
	kdtree.initialize(100, math::loadVector(0, 0, 0));
	kdtree.analyze(&smallHouse, 4);

	if (WRITE_KDTREE_TO_FILE) {
		kdtree.writeToObjFile("../../workspace/test_results/house_kdtree.obj");
	}

	SpherePrimitive outdoorLightGeometry;
	outdoorLightGeometry.setRadius((math::real)10.0);
	outdoorLightGeometry.setPosition(math::loadVector(10.5, 0.0, -20.5));

	SpherePrimitive outdoorTopLightGeometry;
	outdoorTopLightGeometry.setRadius((math::real)10.0);
	outdoorTopLightGeometry.setPosition(math::loadVector(0.0, 25.0, 2));

	// Create scene objects
	SceneObject *smallHouseObject = scene.createSceneObject();
	if (!USE_ACCELERATION_STRUCTURE) smallHouseObject->setGeometry(&smallHouse);
	else smallHouseObject->setGeometry(&kdtree);
	smallHouseObject->setDefaultMaterial(wallMaterial);
	smallHouseObject->setName("House");

	SceneObject *outdoorTopLightObject = scene.createSceneObject();
	outdoorTopLightObject->setGeometry(&outdoorTopLightGeometry);
	outdoorTopLightObject->setDefaultMaterial(&outdoorTopLightMaterial);

	SceneObject *lightSource = scene.createSceneObject();
	lightSource->setGeometry(&outdoorLightGeometry);
	lightSource->setDefaultMaterial(&outdoorLight);

	RandomSampler sampler;

	// Create the camera
	StandardCameraRayEmitterGroup camera;
	camera.setSampler(&sampler);
	camera.setDirection(math::loadVector(-1.0, 0.0, 0.0));
	camera.setPosition(math::loadVector(5.0, 2.0, 0.0));
	camera.setUp(math::loadVector(0.0f, 1.0, 0.0));
	camera.setPlaneDistance(1.0f);
	camera.setPlaneHeight(1.0f);
	camera.setResolutionX(resolutionX);
	camera.setResolutionY(resolutionY);
	camera.setSampleCount(samplesPerPixel);

	// Create the raytracer
	rayTracer.initialize(1000 * MB, 100 * MB, 12, 10000, true);
	rayTracer.setBackgroundColor(getColor(135, 206, 235));
	rayTracer.setDeterministicSeedMode(DETERMINISTIC_SEED_MODE);

	if (TRACE_SINGLE_PIXEL) {
		rayTracer.tracePixel(1094, 768, &scene, &camera);
	}
	else {
		rayTracer.traceAll(&scene, &camera);
	}

	// Output the results to a scene buffer
	SceneBuffer sceneBuffer;
	camera.fillSceneBuffer(&sceneBuffer);

	// Clean up the camera
	camera.destroyRays();
	camera.destroyEmitters();

	std::string fname = createUniqueRenderFilename("small_house_demo", samplesPerPixel);
	std::string imageFname = std::string(RENDER_OUTPUT) + "bitmap/" + fname + ".bmp";
	std::string rawFname = std::string(RENDER_OUTPUT) + "raw/" + fname + ".fpm";

	RawFile rawFile;
	rawFile.writeRawFile(rawFname.c_str(), &sceneBuffer);

	// Apply gamma correction
	sceneBuffer.applyGammaCurve((math::real)(1.0 / 2.2));
	manta::SaveImageData(sceneBuffer.getBuffer(), sceneBuffer.getWidth(), sceneBuffer.getHeight(), imageFname.c_str());

	// Cleanup memory
	sceneBuffer.destroy();
	rayTracer.destroy();
	kdtree.destroy();
	smallHouse.destroy();
	smallHouseObj.destroy();
	shutterObj.destroy();
	shutters.destroy();

	std::cout << "Standard allocator memory leaks:     " << StandardAllocator::Global()->getLedger() << ", " << StandardAllocator::Global()->getCurrentUsage() << std::endl;
}
