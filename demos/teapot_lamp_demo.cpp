#include <demos.h>

#include <iostream>

using namespace manta;

void manta_demo::teapotLampDemo(int samplesPerPixel, int resolutionX, int resolutionY) {
	Scene scene;

	// Load all object files
	ObjFileLoader lampObj;
	bool result = lampObj.readObjFile(MODEL_PATH "lamp2.obj");

	ObjFileLoader teapotObj;
	result = teapotObj.readObjFile(MODEL_PATH "lamp_teapot.obj");

	ObjFileLoader lampBlockObj;
	result = lampBlockObj.readObjFile(MODEL_PATH "lampblock.obj");

	RayTracer rayTracer;

	// Create all materials
	PhongMaterial *wallMaterial = rayTracer.getMaterialManager()->newMaterial<PhongMaterial>();
	wallMaterial->setSpecularPower((math::real)1.0);
	wallMaterial->setEmission(math::constants::Zero);
	wallMaterial->setDiffuseColor(getColor(200, 200, 200));
	wallMaterial->setSpecularColor(math::constants::Zero);

	SimpleSpecularDiffuseMaterial *lampLightMaterial = rayTracer.getMaterialManager()->newMaterial<SimpleSpecularDiffuseMaterial>();
	lampLightMaterial->setEmission(math::mul(getColor(255, 197, 143), math::loadScalar(30.0)));
	lampLightMaterial->setDiffuseColor(math::constants::Zero);
	lampLightMaterial->setSpecularColor(math::constants::Zero);

	PhongMaterial *teapotMaterial = rayTracer.getMaterialManager()->newMaterial<PhongMaterial>();
	teapotMaterial->setSpecularPower((math::real)2.0);
	teapotMaterial->setEmission(math::constants::Zero);
	teapotMaterial->setDiffuseColor(getColor(0xFF, 0x08, 0x14));
	teapotMaterial->setSpecularColor(getColor(100, 100, 100));

	SimpleSpecularDiffuseMaterial *groundMaterial = rayTracer.getMaterialManager()->newMaterial<SimpleSpecularDiffuseMaterial>();
	groundMaterial->setEmission(math::constants::Zero);
	groundMaterial->setDiffuseColor(math::constants::Zero);
	groundMaterial->setSpecularColor(getColor(100, 100, 100));

	// Create all scene geometry
	Mesh lamp;
	lamp.loadObjFileData(&lampObj, rayTracer.getMaterialManager(), wallMaterial->getIndex());
	lamp.setFastIntersectEnabled(true);
	lamp.setFastIntersectRadius((math::real)2.123);
	lamp.setFastIntersectPosition(math::loadVector(-0.06430, 1.86833, -2.96564));

	//Octree lampOctree;
	//lampOctree.initialize(2.5, math::loadVector(-0.06430, 1.86833, -2.96564));
	//lampOctree.analyze(&lamp, 4);

	Mesh lampBlock;
	lampBlock.loadObjFileData(&lampBlockObj);
	lampBlock.setFastIntersectEnabled(false);
	lampBlock.setFastIntersectRadius((math::real)4.0);

	Mesh teapot;
	teapot.loadObjFileData(&teapotObj, rayTracer.getMaterialManager(), teapotMaterial->getIndex());
	teapot.setFastIntersectEnabled(true);
	teapot.setFastIntersectRadius((math::real)2.0);
	teapot.setFastIntersectPosition(math::loadVector(-0.5724, 1.02483, -0.04969));

	teapot.merge(&lamp);
	//teapot.merge(&lampBlock);

	Octree teapotOctree;
	teapotOctree.initialize(4, math::constants::Zero);
	teapotOctree.analyze(&teapot, 25);
	//teapotOctree.writeToObjFile("../../workspace/test_results/teapot_lamp_octree.obj", nullptr);

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
	groundGeometry.setRadius((math::real)5000.0);
	groundGeometry.setPosition(math::loadVector(0.0, -5000.001, 0));

	constexpr bool useOctree = true;

	// Create scene objects
	//SceneObject *lampObject = scene.createSceneObject();
	//if (useOctree) lampObject->setGeometry(&lampOctree);
	//else lampObject->setGeometry(&lamp);
	//lampObject->setDefaultMaterial(wallMaterial);
	//lampObject->setName("Lamp");

	SceneObject *bulbObject = scene.createSceneObject();
	bulbObject->setGeometry(&bulb);
	bulbObject->setDefaultMaterial(lampLightMaterial);
	bulbObject->setName("Bulb");

	SceneObject *teapotObject = scene.createSceneObject();
	if (useOctree) {
		teapotObject->setGeometry(&teapotOctree);
	}
	else {
		teapotObject->setGeometry(&teapot);
	}
	teapotObject->setDefaultMaterial(teapotMaterial);
	teapotObject->setName("Teapot");

	SceneObject *ground = scene.createSceneObject();
	ground->setGeometry(&groundGeometry);
	ground->setDefaultMaterial(wallMaterial);
	ground->setName("Ground");

	//SceneObject *lampBlockObject = scene.createSceneObject();
	//lampBlockObject->setGeometry(&lampBlock);
	//lampBlockObject->setMaterial(&wallMaterial);
	//lampBlockObject->setName("LampBlock");

	// Create the camera
	SSCameraRayEmitterGroup camera;
	camera.setSamplingWidth(3);
	camera.setDirection(math::loadVector(-1.0, 0.0, 0.0));
	camera.setPosition(math::loadVector(7.0, 2.0, 0.0));
	camera.setUp(math::loadVector(0.0f, 1.0, 0.0));
	camera.setPlaneDistance(1.0f);
	camera.setPlaneHeight(1.0f);
	camera.setResolutionX(resolutionX);
	camera.setResolutionY(resolutionY);
	camera.setSamplesPerPixel(samplesPerPixel);

	// Create the raytracer
	rayTracer.initialize(1000 * MB, 100 * MB, 12, 10000, true);
	rayTracer.setBackgroundColor(getColor(0, 0, 0));
	rayTracer.setDeterministicSeedMode(false);
	rayTracer.setPathRecordingOutputDirectory("../../workspace/diagnostics/");
	//rayTracer.tracePixel(684, 909, &scene, &camera);
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

	// Output the results to a scene buffer
	SceneBuffer sceneBuffer;
	camera.fillSceneBuffer(&sceneBuffer);

	// Clean up the camera
	camera.destroyRays();
	camera.destroyEmitters();

	std::string fname = createUniqueRenderFilename("teapot_lamp_demo", samplesPerPixel);
	std::string imageFname = std::string(RENDER_OUTPUT) + "bitmap/" + fname + ".bmp";
	std::string rawFname = std::string(RENDER_OUTPUT) + "raw/" + fname + ".fpm";

	RawFile rawFile;
	rawFile.writeRawFile(rawFname.c_str(), &sceneBuffer);
	//editImage(&sceneBuffer, imageFname);

	sceneBuffer.applyGammaCurve((math::real)(1.0 / 2.2));
	manta::SaveImageData(sceneBuffer.getBuffer(), sceneBuffer.getWidth(), sceneBuffer.getHeight(), imageFname.c_str());

	sceneBuffer.destroy();
	rayTracer.destroy();

	lamp.destroy();
	lampBlock.destroy();
	lampBlockObj.destroy();
	lampObj.destroy();
	teapot.destroy();
	teapotObj.destroy();
	teapotOctree.destroy();

	std::cout << "Standard allocator memory leaks:     " << StandardAllocator::Global()->getLedger() << ", " << StandardAllocator::Global()->getCurrentUsage() << std::endl;
}
