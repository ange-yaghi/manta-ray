#include <demos.h>

#include <iostream>

using namespace manta;

void manta_demo::simpleRoomDemo(int samplesPerPixel, int resolutionX, int resolutionY) {
	Scene scene;

	// Load all object files
	ObjFileLoader smallHouseObj;
	bool result = smallHouseObj.readObjFile(MODEL_PATH "small_house_unsealed.obj");

	ObjFileLoader tableObj;
	result = tableObj.readObjFile(MODEL_PATH "table.obj");

	ObjFileLoader shutterObj;
	result = shutterObj.readObjFile(MODEL_PATH "room_shutters.obj");

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

	//SceneObject *shuttersObject = scene.createSceneObject();
	//shuttersObject->setGeometry(&shutters);
	//shuttersObject->setMaterial(&wallMaterial);
	//shuttersObject->setName("Shutters");

	//SceneObject *ground = scene.createSceneObject();
	//ground->setGeometry(&groundGeometry);
	//ground->setMaterial(&groundMaterial);

	SceneObject *outdoorTopLightObject = scene.createSceneObject();
	outdoorTopLightObject->setGeometry(&outdoorTopLightGeometry);
	outdoorTopLightObject->setMaterial(&outdoorTopLightMaterial);

	SceneObject *lightSource = scene.createSceneObject();
	lightSource->setGeometry(&outdoorLightGeometry);
	lightSource->setMaterial(&outdoorLight);

	// Create the camera
	SSCameraRayEmitterGroup camera;
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
	//rayTracer.setDeterministicSeedMode(true);
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
	//editImage(&sceneBuffer, imageFname);

	// Apply gamma correction
	sceneBuffer.applyGammaCurve((math::real)(1.0 / 2.2));
	manta::SaveImageData(sceneBuffer.getBuffer(), sceneBuffer.getWidth(), sceneBuffer.getHeight(), imageFname.c_str());

	sceneBuffer.destroy();
	rayTracer.destroy();
}
