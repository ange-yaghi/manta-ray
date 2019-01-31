#include <demos.h>

#include <iostream>

using namespace manta;

void manta_demo::complexRoomDemo(int samplesPerPixel, int resolutionX, int resolutionY) {
	Scene scene;

	std::cout << sizeof(Octree) << std::endl;

	// Load all object files
	ObjFileLoader stressSpidersObj;
	bool result = stressSpidersObj.readObjFile(MODEL_PATH "complex_room.obj");

	ObjFileLoader roomShuttersObj;
	result = roomShuttersObj.readObjFile(MODEL_PATH "complex_room_shutters.obj");

	RayTracer rayTracer;

	DielectricMediaInterface glassFresnel;
	glassFresnel.setIorIncident(1.0);
	glassFresnel.setIorTransmitted(1.6);

	// Create all materials
	PhongPhongBilayerMaterial *wallMaterial = rayTracer.getMaterialManager()->newMaterial<PhongPhongBilayerMaterial>();
	wallMaterial->setName("WallMaterial");
	wallMaterial->setEmission(math::constants::Zero);
	wallMaterial->setDiffuseColor(getColor(0xFF, 0xFF, 0xFF));
	wallMaterial->setSpecularColor(getColor(0xFF, 0xFF, 0xFF));
	wallMaterial->setSurfaceTransmission((math::real)0.3);
	wallMaterial->getDiffuseBSDF()->setPower((math::real)2.0);
	wallMaterial->getSpecularBSDF()->setPower((math::real)4.0);
	wallMaterial->getSpecularBSDF()->setMediaInterface(&glassFresnel);
	//wallMaterial->setGloss((math::real)0.95);

	PhongPhongBilayerMaterial *floorMaterial = rayTracer.getMaterialManager()->newMaterial<PhongPhongBilayerMaterial>();
	floorMaterial->setName("FloorMaterial");
	floorMaterial->setEmission(math::constants::Zero);
	floorMaterial->setDiffuseColor(getColor(0xFF, 0xFF, 0xFF));
	floorMaterial->setSpecularColor(getColor(0xFF, 0xFF, 0xFF));
	floorMaterial->getDiffuseBSDF()->setPower((math::real)2.0);
	floorMaterial->getSpecularBSDF()->setPower((math::real)32.0);
	floorMaterial->getSpecularBSDF()->setMediaInterface(&glassFresnel);
	floorMaterial->setSurfaceTransmission((math::real)0.5);
	//floorMaterial->setGloss((math::real)0.95);

	PhongPhongBilayerMaterial *carpetMaterial = rayTracer.getMaterialManager()->newMaterial<PhongPhongBilayerMaterial>();
	carpetMaterial->setName("CarpetMaterial");
	carpetMaterial->setEmission(math::constants::Zero);
	carpetMaterial->setDiffuseColor(getColor(0xdc, 0xb4, 0x84));
	carpetMaterial->setSpecularColor(getColor(0xdc, 0xb4, 0x84));
	carpetMaterial->getDiffuseBSDF()->setPower((math::real)2.0);
	carpetMaterial->getSpecularBSDF()->setPower((math::real)4.0);
	carpetMaterial->getSpecularBSDF()->setMediaInterface(&glassFresnel);
	carpetMaterial->setSurfaceTransmission((math::real)0.3);
	//carpetMaterial->setGloss((math::real)0.95);

	PhongPhongBilayerMaterial *tableSteel = rayTracer.getMaterialManager()->newMaterial<PhongPhongBilayerMaterial>();
	tableSteel->setName("TableSteel");
	tableSteel->setEmission(math::constants::Zero);
	tableSteel->setDiffuseColor(getColor(0xCC, 0xCC, 0xCC));
	tableSteel->setSpecularColor(getColor(0xff, 0xff, 0xff));
	tableSteel->getDiffuseBSDF()->setPower((math::real)128.0);
	tableSteel->getSpecularBSDF()->setPower((math::real)1024.0);
	tableSteel->getSpecularBSDF()->setMediaInterface(&glassFresnel);
	tableSteel->setSurfaceTransmission(0.95);
	//tableSteel->setGloss((math::real)0.95);

	PhongPhongBilayerMaterial *tableTop = rayTracer.getMaterialManager()->newMaterial<PhongPhongBilayerMaterial>();
	tableTop->setName("TableTop");
	tableTop->setEmission(math::constants::Zero);
	tableTop->setDiffuseColor(getColor(0x30, 0x26, 0x21));
	tableTop->setSpecularColor(getColor(0xff, 0xff, 0xff));
	tableTop->getDiffuseBSDF()->setPower((math::real)4.0);
	tableTop->getSpecularBSDF()->setPower((math::real)512.0);
	tableTop->getSpecularBSDF()->setMediaInterface(&glassFresnel);
	tableTop->setSurfaceTransmission((math::real)0.30);
	//tableTop->setGloss((math::real)0.95);

	PhongPhongBilayerMaterial *book = rayTracer.getMaterialManager()->newMaterial<PhongPhongBilayerMaterial>();
	book->setName("Book");
	book->setEmission(math::constants::Zero);
	book->setDiffuseColor(getColor(0xFF, 0xFF, 0xFF));
	book->setSpecularColor(getColor(0xFF, 0xFF, 0xff));
	tableTop->getDiffuseBSDF()->setPower((math::real)8.0);
	tableTop->getSpecularBSDF()->setPower((math::real)512.0);
	tableTop->getSpecularBSDF()->setMediaInterface(&glassFresnel);
	book->setSurfaceTransmission((math::real)0.4);

	SimpleLambertMaterial *groundMaterial = rayTracer.getMaterialManager()->newMaterial<SimpleLambertMaterial>();
	groundMaterial->setEmission(math::constants::Zero);
	groundMaterial->setDiffuseColor(getColor(255, 255, 255));
	//groundMaterial->setSpecularColor(math::constants::Zero);

	SimpleLambertMaterial outdoorTopLightMaterial;
	outdoorTopLightMaterial.setEmission(math::loadVector(5, 5, 5));
	outdoorTopLightMaterial.setDiffuseColor(math::constants::Zero);
	//outdoorTopLightMaterial.setSpecularColor(math::constants::Zero);

	// Create all scene geometry
	Mesh stressSpiders;
	stressSpiders.loadObjFileData(&stressSpidersObj, rayTracer.getMaterialManager(), wallMaterial->getIndex());
	stressSpiders.setFastIntersectEnabled(false);
	//stressSpiders.setFastIntersectRadius((math::real)2.123);
	//stressSpiders.setFastIntersectPosition(math::loadVector(-0.06430, 1.86833, -2.96564));

	std::cout << "Scene vertices/faces: " << stressSpiders.getVertexCount() << "/" << stressSpiders.getFaceCount() << std::endl;

	Mesh roomShutters;
	roomShutters.loadObjFileData(&roomShuttersObj, rayTracer.getMaterialManager(), wallMaterial->getIndex());
	roomShutters.setFastIntersectEnabled(false);

	//stressSpiders.merge(&roomShutters);
	roomShutters.destroy();
	//stressSpiders.precomputeValues();

	stressSpidersObj.destroy();
	roomShuttersObj.destroy();

	math::Vector cameraPos = math::loadVector(5.28056, 2.06530, 5.21611);
	math::Vector target = math::loadVector(-0.45522, 1.27649, 0.82292);

	cameraPos = math::loadVector(-3.70285, 1.59896, 6.71528);
	target = math::loadVector(-3.70285, 1.34146, 2.49088);

	cameraPos = math::loadVector(3.42581, 5.49441, 5.17956);
	target = math::loadVector(-4.03742, 1.24095, -2.2882);

	math::Vector dir = math::normalize(math::sub(target, cameraPos));

	LightRay sampleRay;
	sampleRay.setDirection(dir);
	sampleRay.setSource(cameraPos);

	//Octree stressSpidersOctree;
	//stressSpidersOctree.initialize(32, math::constants::Zero);
	//stressSpidersOctree.analyze(&stressSpiders, 10); //25

	KDTree kdtree;
	kdtree.initialize(100.0, math::constants::Zero);
	kdtree.analyze(&stressSpiders, 4);

	//std::cout << "Octree faces: " << stressSpidersOctree.countFaces() << std::endl;
	//std::cout << "Leaf count: " << stressSpidersOctree.countLeaves() << std::endl;

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
		stressSpidersObject->setGeometry(&kdtree);
	}
	else {
		stressSpidersObject->setGeometry(&stressSpiders);
	}
	stressSpidersObject->setDefaultMaterial(wallMaterial);
	stressSpidersObject->setName("StressSpiders");

	//SceneObject *ground = scene.createSceneObject();
	//ground->setGeometry(&groundGeometry);
	//ground->setDefaultMaterial(groundMaterial);
	//ground->setName("Ground");

	//SceneObject *outdoorTopLightObject = scene.createSceneObject();
	//outdoorTopLightObject->setGeometry(&outdoorTopLightGeometry);
	//outdoorTopLightObject->setDefaultMaterial(&outdoorTopLightMaterial);
	//outdoorTopLightObject->setName("MainLamp");

	math::Vector up = math::loadVector(0.0f, 1.0, 0.0);
	up = math::cross(math::cross(dir, up), dir);
	up = math::normalize(up);

	// Create the camera
	constexpr bool regularCamera = true;
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

	GridSampler sampler;
	sampler.setGridWidth(3);

	RandomSampler randomSampler;

	if (regularCamera) {
		StandardCameraRayEmitterGroup *camera = new StandardCameraRayEmitterGroup;
		//camera->setSamplingWidth(3);
		camera->setDirection(dir);
		camera->setPosition(cameraPos);
		camera->setUp(up);
		camera->setPlaneDistance(1.0f);
		camera->setPlaneHeight(1.0f); // 0.75
		camera->setResolutionX(resolutionX);
		camera->setResolutionY(resolutionY);
		camera->setSampleCount(samplesPerPixel);
		camera->setSampler(&randomSampler);

		group = camera;
	}
	else {
		math::real lensHeight = 1.0;
		math::real focusDistance = 11.0;

		Aperture *aperture = lens.getAperture();
		aperture->setRadius((math::real)0.05);
		lens.setFocus(focusDistance);

		LensCameraRayEmitterGroup *camera = new LensCameraRayEmitterGroup;
		camera->setSampler(&sampler);
		camera->setDirection(math::normalize(math::sub(target, cameraPos)));
		camera->setPosition(cameraPos);
		camera->setLens(&lens);
		camera->setResolutionX(resolutionX);
		camera->setResolutionY(resolutionY);
		camera->setSampleCount(samplesPerPixel);

		group = camera;
	}

	// Create the raytracer
	rayTracer.initialize(1000 * MB, 100 * MB, 12, 10000, true);
	rayTracer.setBackgroundColor(getColor(255, 255, 255));
	rayTracer.setDeterministicSeedMode(false);
	rayTracer.setPathRecordingOutputDirectory("../../workspace/diagnostics/");
	//rayTracer.tracePixel(583, 818, &scene, group);
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

	//stressSpidersOctree.destroy();
	stressSpiders.destroy();
	kdtree.destroy();

	std::cout << "Standard allocator memory leaks:     " << StandardAllocator::Global()->getLedger() << ", " << StandardAllocator::Global()->getCurrentUsage() << std::endl;
}
