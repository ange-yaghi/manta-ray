#include <demos.h>

#include <iostream>

using namespace manta;

void manta_demo::complexRoomDemo(int samplesPerPixel, int resolutionX, int resolutionY) {
	Scene scene;

	// Load all object files
	ObjFileLoader stressSpidersObj;
	bool result = stressSpidersObj.readObjFile(MODEL_PATH "complex_room.obj");

	RayTracer rayTracer;

	// Create all materials
	SimpleSpecularDiffuseMaterial *spiderMaterial = rayTracer.getMaterialManager()->newMaterial<SimpleSpecularDiffuseMaterial>();
	spiderMaterial->setEmission(math::mul(getColor(0xFF, 0x08, 0x14), math::loadScalar(0.0)));
	spiderMaterial->setDiffuseColor(getColor(0xf1, 0xc4, 0x0f));
	spiderMaterial->setDiffuseColor(getColor(0xD3, 0xD3, 0xD3));
	spiderMaterial->setSpecularColor(getColor(70, 70, 70));
	spiderMaterial->setSpecularColor(getColor(0x10, 0x10, 0x10));
	spiderMaterial->setGloss((math::real)0.1);

	SimpleSpecularDiffuseMaterial *groundMaterial = rayTracer.getMaterialManager()->newMaterial<SimpleSpecularDiffuseMaterial>();
	groundMaterial->setEmission(math::constants::Zero);
	groundMaterial->setDiffuseColor(getColor(255, 255, 255));
	groundMaterial->setSpecularColor(math::constants::Zero);

	SimpleSpecularDiffuseMaterial outdoorTopLightMaterial;
	outdoorTopLightMaterial.setEmission(math::loadVector(5, 5, 5));
	outdoorTopLightMaterial.setDiffuseColor(math::constants::Zero);
	outdoorTopLightMaterial.setSpecularColor(math::constants::Zero);

	// Create all scene geometry
	Mesh stressSpiders;
	stressSpiders.loadObjFileData(&stressSpidersObj, spiderMaterial->getIndex());
	stressSpiders.setFastIntersectEnabled(false);
	//stressSpiders.setFastIntersectRadius((math::real)2.123);
	//stressSpiders.setFastIntersectPosition(math::loadVector(-0.06430, 1.86833, -2.96564));

	stressSpidersObj.destroy();

	Octree stressSpidersOctree;
	stressSpidersOctree.initialize(32, math::constants::Zero);
	stressSpidersOctree.analyze(&stressSpiders, 25);
	//stressSpidersOctree.writeToObjFile("../../workspace/test_results/complex_room_octree.obj", nullptr);

	SpherePrimitive outdoorTopLightGeometry;
	outdoorTopLightGeometry.setRadius((math::real)10.0);
	//outdoorTopLightGeometry.setRadius((math::real)20.0);
	outdoorTopLightGeometry.setPosition(math::loadVector(19.45842, 12.42560, -13.78918));

	SpherePrimitive groundGeometry;
	groundGeometry.setRadius((math::real)50000.0);
	groundGeometry.setPosition(math::loadVector(0.0, -50000.001, 0));

	constexpr bool useOctree = true;

	// Create scene objects
	SceneObject *stressSpidersObject = scene.createSceneObject();
	if (useOctree) {
		stressSpidersObject->setGeometry(&stressSpidersOctree);
	}
	else {
		stressSpidersObject->setGeometry(&stressSpiders);
	}
	stressSpidersObject->setDefaultMaterial(spiderMaterial);
	stressSpidersObject->setName("StressSpiders");

	//SceneObject *ground = scene.createSceneObject();
	//ground->setGeometry(&groundGeometry);
	//ground->setDefaultMaterial(groundMaterial);
	//ground->setName("Ground");

	//SceneObject *outdoorTopLightObject = scene.createSceneObject();
	//outdoorTopLightObject->setGeometry(&outdoorTopLightGeometry);
	//outdoorTopLightObject->setDefaultMaterial(&outdoorTopLightMaterial);
	//outdoorTopLightObject->setName("MainLamp");

	math::Vector cameraPos = math::loadVector(5.28056, 2.06530, 5.21611);
	math::Vector target = math::loadVector(-0.45522, 1.27649, 0.82292);

	math::Vector up = math::loadVector(0.0f, 1.0, 0.0);
	math::Vector dir = math::normalize(math::sub(target, cameraPos));
	up = math::cross(math::cross(dir, up), dir);
	up = math::normalize(up);

	// Create the camera
	constexpr bool regularCamera = false;
	CameraRayEmitterGroup *group;
	manta::SimpleLens lens;
	lens.initialize();
	lens.setPosition(cameraPos);
	lens.setDirection(dir);
	lens.setUp(up);
	lens.setRadius(1.0);
	lens.setSensorResolutionX(resolutionX);
	lens.setSensorResolutionY(resolutionY);
	lens.setSensorHeight(72.0);
	lens.setSensorWidth(72.0 * (resolutionX / (math::real)resolutionY));
	lens.update();

	if (regularCamera) {
		SSCameraRayEmitterGroup *camera = new SSCameraRayEmitterGroup;
		camera->setSamplingWidth(1);
		camera->setDirection(dir);
		camera->setPosition(cameraPos);
		camera->setUp(up);
		camera->setPlaneDistance(1.0f);
		camera->setPlaneHeight(1.0f); // 0.75
		camera->setResolutionX(resolutionX);
		camera->setResolutionY(resolutionY);
		camera->setSamplesPerPixel(samplesPerPixel);

		group = camera;
	}
	else {
		math::real lensHeight = 1.0;
		math::real focusDistance = 11.0;

		Aperture *aperture = lens.getAperture();
		aperture->setSize((math::real)0.05);
		lens.setFocus(focusDistance);

		LensCameraRayEmitterGroup *camera = new LensCameraRayEmitterGroup;
		camera->setDirection(math::normalize(math::sub(target, cameraPos)));
		camera->setPosition(cameraPos);
		camera->setLens(&lens);
		camera->setResolutionX(resolutionX);
		camera->setResolutionY(resolutionY);
		camera->setSamplesPerPixel(1);
		camera->setExplicitSampleCount(samplesPerPixel);

		group = camera;
	}

	// Create the raytracer
	rayTracer.initialize(1000 * MB, 100 * MB, 12, 10000, true);
	rayTracer.setBackgroundColor(getColor(255, 255, 255));
	//rayTracer.setDeterministicSeedMode(true);
	//rayTracer.setPathRecordingOutputDirectory("../../workspace/diagnostics/");
	//rayTracer.tracePixel(1604, 615, &scene, group);
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

	rayTracer.traceAll(&scene, group);

	// Output the results to a scene buffer
	SceneBuffer sceneBuffer;
	group->fillSceneBuffer(&sceneBuffer);

	// Clean up the camera
	group->destroyRays();
	group->destroyEmitters();

	std::string fname = createUniqueRenderFilename("complex_room_demo", samplesPerPixel);
	std::string imageFname = std::string(RENDER_OUTPUT) + "bitmap/" + fname + ".bmp";
	std::string rawFname = std::string(RENDER_OUTPUT) + "raw/" + fname + ".fpm";

	RawFile rawFile;
	rawFile.writeRawFile(rawFname.c_str(), &sceneBuffer);
	//editImage(&sceneBuffer, imageFname);

	sceneBuffer.applyGammaCurve((math::real)(1.0 / 2.2));
	manta::SaveImageData(sceneBuffer.getBuffer(), sceneBuffer.getWidth(), sceneBuffer.getHeight(), imageFname.c_str());

	sceneBuffer.destroy();
	rayTracer.destroy();

	stressSpidersOctree.destroy();
	stressSpiders.destroy();

	std::cout << "Standard allocator memory leaks:     " << StandardAllocator::Global()->getLedger() << ", " << StandardAllocator::Global()->getCurrentUsage() << std::endl;
}
