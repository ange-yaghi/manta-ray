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
	outdoorLight.setEmission(math::loadVector(18.f, 16.f, 16.f));
	outdoorLight.setReflectance(math::constants::Zero);

	SimpleBSDFMaterial outdoorTopLightMaterial;
	outdoorTopLightMaterial.setEmission(math::loadVector(20.f, 20.f, 22.f));
	outdoorTopLightMaterial.setReflectance(math::constants::Zero);

	SimpleBSDFMaterial *tableMaterial = rayTracer.getMaterialManager()->newMaterial<SimpleBSDFMaterial>();
	tableMaterial->setName("TableMaterial");
	tableMaterial->setReflectance(getColor(78, 46, 40));
	tableMaterial->setBSDF(&lambert);

	SimpleBSDFMaterial *groundMaterial = rayTracer.getMaterialManager()->newMaterial<SimpleBSDFMaterial>();
	groundMaterial->setName("GroundMaterial");
	groundMaterial->setReflectance(math::mul(math::loadVector(78, 46, 40), math::loadScalar(0.001f)));
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
	kdtree.initialize(100.f, math::loadVector(0, 0, 0));
	kdtree.analyze(&smallHouse, 4);

	if (WRITE_KDTREE_TO_FILE) {
		kdtree.writeToObjFile("../../workspace/test_results/house_kdtree.obj");
	}

	SpherePrimitive outdoorLightGeometry;
	outdoorLightGeometry.setRadius((math::real)10.0);
	outdoorLightGeometry.setPosition(math::loadVector(10.5f, 0.0f, -20.5f));

	SpherePrimitive outdoorTopLightGeometry;
	outdoorTopLightGeometry.setRadius((math::real)10.0);
	outdoorTopLightGeometry.setPosition(math::loadVector(0.0f, 25.0f, 2.f));

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
	camera.setDirection(math::loadVector(-1.0f, 0.0f, 0.0f));
	camera.setPosition(math::loadVector(5.0f, 2.0f, 0.0f));
	camera.setUp(math::loadVector(0.0f, 1.0f, 0.0f));
	camera.setPlaneDistance(1.0f);
	camera.setPlaneHeight(1.0f);
	camera.setResolutionX(resolutionX);
	camera.setResolutionY(resolutionY);
	camera.setSampleCount(samplesPerPixel);

	// Output the results to a scene buffer
	ImagePlane sceneBuffer;

	// Create the raytracer
	rayTracer.initialize(200 * MB, 100 * MB, 12, 100, true);
	rayTracer.setBackgroundColor(getColor(135, 206, 235));
	rayTracer.setDeterministicSeedMode(DETERMINISTIC_SEED_MODE);

	if (TRACE_SINGLE_PIXEL) {
		rayTracer.tracePixel(1094, 768, &scene, &camera, &sceneBuffer);
	}
	else {
		rayTracer.traceAll(&scene, &camera, &sceneBuffer);
	}

	std::string fname = createUniqueRenderFilename("small_house_demo", samplesPerPixel);
	std::string imageFname = std::string(RENDER_OUTPUT) + "bitmap/" + fname + ".jpg";
	std::string rawFname = std::string(RENDER_OUTPUT) + "raw/" + fname + ".fpm";

	RawFile rawFile;
	rawFile.writeRawFile(rawFname.c_str(), &sceneBuffer);

	// Apply gamma correction
	writeJpeg(imageFname.c_str(), &sceneBuffer, 95);

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
