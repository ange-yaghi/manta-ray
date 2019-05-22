#include <demos.h>

#include <iostream>

using namespace manta;

void manta_demo::complexRoomDemo(int samplesPerPixel, int resolutionX, int resolutionY) {
	// Top-level parameters
	constexpr bool BLOCK_ALL_LIGHT = false;
	constexpr bool LENS_SIMULATION = false;
	constexpr bool USE_ACCELERATION_STRUCTURE = true;
	constexpr bool DETERMINISTIC_SEED_MODE = false;
	constexpr bool TRACE_SINGLE_PIXEL = false;

	Scene scene;

	// Load all object files
	ObjFileLoader complexRoomObj;
	bool result = complexRoomObj.readObjFile(MODEL_PATH "complex_room.obj");

	ObjFileLoader roomShuttersObj;
	result &= roomShuttersObj.readObjFile(MODEL_PATH "complex_room_shutters.obj");

	if (!result) {
		std::cout << "Could not open geometry file(s)" << std::endl;

		complexRoomObj.destroy();
		roomShuttersObj.destroy();

		return;
	}

	RayTracer rayTracer;
	
	DielectricMediaInterface glassFresnel;
	glassFresnel.setIorIncident(1.0f);
	glassFresnel.setIorTransmitted(1.6f);

	// Create all materials
	LambertianBSDF lambert;

	// Wall material
	PhongDistribution wallCoating;
	wallCoating.setPower((math::real)8.0);
	BilayerBSDF wallBSDF;
	wallBSDF.setDiffuse(getColor(0xFF, 0xFF, 0xFF));
	wallBSDF.setSpecularAtNormal(getColor(0x00, 0x00, 0x00));
	wallBSDF.setCoatingDistribution(wallCoating.getMainOutput());
	wallBSDF.setDiffuseMaterial(&lambert);
	SimpleBSDFMaterial *wallMaterial = rayTracer.getMaterialManager()->newMaterial<SimpleBSDFMaterial>();
	wallMaterial->setName("WallMaterial");
	wallMaterial->setBSDF(&wallBSDF);

	// Floor material
	PhongDistribution floorCoating;
	floorCoating.setPower((math::real)64.0);
	BilayerBSDF floorBSDF;
	floorBSDF.setDiffuse(getColor(0xFF, 0xFF, 0xFF));
	floorBSDF.setSpecularAtNormal(math::loadVector(1.0f, 1.0f, 1.0f));
	floorBSDF.setCoatingDistribution(wallCoating.getMainOutput());
	floorBSDF.setDiffuseMaterial(&lambert);
	SimpleBSDFMaterial *floorMaterial = rayTracer.getMaterialManager()->newMaterial<SimpleBSDFMaterial>();
	floorMaterial->setName("FloorMaterial");
	floorMaterial->setBSDF(&floorBSDF);

	// Floor material
	PhongDistribution carpetCoating;
	carpetCoating.setPower((math::real)4.0);
	BilayerBSDF carpetBSDF;
	carpetBSDF.setDiffuse(getColor(0xdc, 0xb4, 0x84));
	carpetBSDF.setSpecularAtNormal(math::loadVector(0.0f, 0.0f, 0.0f));
	carpetBSDF.setCoatingDistribution(wallCoating.getMainOutput());
	carpetBSDF.setDiffuseMaterial(&lambert);
	SimpleBSDFMaterial *carpetMaterial = rayTracer.getMaterialManager()->newMaterial<SimpleBSDFMaterial>();
	carpetMaterial->setName("CarpetMaterial");
	carpetMaterial->setBSDF(&carpetBSDF);

	// Steel material
	PhongDistribution tableSteelCoating;
	tableSteelCoating.setPower((math::real)1024.0);
	BilayerBSDF tableSteelBSDF;
	tableSteelBSDF.setDiffuse(getColor(0x00, 0x00, 0x00));
	tableSteelBSDF.setSpecularAtNormal(math::loadVector(1.0f, 1.0f, 1.0f));
	tableSteelBSDF.setCoatingDistribution(tableSteelCoating.getMainOutput());
	tableSteelBSDF.setDiffuseMaterial(&lambert);
	SimpleBSDFMaterial *tableSteel = rayTracer.getMaterialManager()->newMaterial<SimpleBSDFMaterial>();
	tableSteel->setName("TableSteel");
	tableSteel->setBSDF(&tableSteelBSDF);

	// Table top material
	PhongDistribution tableTopCoating;
	tableTopCoating.setPower((math::real)512.0);
	BilayerBSDF tableTopBSDF;
	tableTopBSDF.setDiffuse(getColor(0x30, 0x26, 0x21));
	tableTopBSDF.setSpecularAtNormal(math::loadVector(0.2f, 0.2f, 0.2f));
	tableTopBSDF.setCoatingDistribution(tableTopCoating.getMainOutput());
	tableTopBSDF.setDiffuseMaterial(&lambert);
	SimpleBSDFMaterial *tableTop = rayTracer.getMaterialManager()->newMaterial<SimpleBSDFMaterial>();
	tableTop->setName("TableTop");
	tableTop->setBSDF(&tableTopBSDF);

	// Book material
	PhongDistribution bookCoating;
	bookCoating.setPower((math::real)4.0);
	BilayerBSDF bookBSDF;
	bookBSDF.setDiffuse(getColor(0xFF, 0xFF, 0xFF));
	bookBSDF.setSpecularAtNormal(math::loadVector(0.0f, 0.0f, 0.0f));
	bookBSDF.setCoatingDistribution(tableTopCoating.getMainOutput());
	bookBSDF.setDiffuseMaterial(&lambert);
	SimpleBSDFMaterial *book = rayTracer.getMaterialManager()->newMaterial<SimpleBSDFMaterial>();
	book->setName("Book");
	book->setBSDF(&bookBSDF);

	// Create all scene geometry
	Mesh roomGeometry;
	roomGeometry.loadObjFileData(&complexRoomObj, rayTracer.getMaterialManager());
	roomGeometry.setFastIntersectEnabled(false);

	if (BLOCK_ALL_LIGHT) {
		Mesh roomShutters;
		roomShutters.loadObjFileData(&roomShuttersObj, rayTracer.getMaterialManager(), wallMaterial->getIndex());
		roomShutters.setFastIntersectEnabled(false);

		roomGeometry.merge(&roomShutters);
		roomShutters.destroy();
	}

	complexRoomObj.destroy();
	roomShuttersObj.destroy();

	math::Vector cameraPos = math::loadVector(5.28056f, 2.06530f, 5.21611f);
	math::Vector target = math::loadVector(-0.45522f, 1.27649f, 0.82292f);

	cameraPos = math::loadVector(-3.70285f, 1.59896f, 6.71528f);
	target = math::loadVector(-3.70285f, 1.34146f, 2.49088f);

	cameraPos = math::loadVector(3.42581f, 5.49441f, 5.17956f);
	target = math::loadVector(-4.03742f, 1.24095f, -2.2882f);

	math::Vector dir = math::normalize(math::sub(target, cameraPos));

	LightRay sampleRay;
	sampleRay.setDirection(dir);
	sampleRay.setSource(cameraPos);

	KDTree kdtree;
	kdtree.initialize(100.0f, math::constants::Zero);
	kdtree.analyzeWithProgress(&roomGeometry, 4);

	SpherePrimitive outdoorTopLightGeometry;
	outdoorTopLightGeometry.setRadius((math::real)10.0);
	outdoorTopLightGeometry.setPosition(math::loadVector(19.45842f, 12.42560f, -13.78918f));

	// Create scene objects
	SceneObject *roomObject = scene.createSceneObject();
	if (USE_ACCELERATION_STRUCTURE) roomObject->setGeometry(&kdtree);
	else roomObject->setGeometry(&roomGeometry);

	roomObject->setDefaultMaterial(wallMaterial);
	roomObject->setName("ComplexRoom");

	// Create the camera
	math::Vector up = math::loadVector(0.0f, 1.0f, 0.0f);
	up = math::cross(math::cross(dir, up), dir);
	up = math::normalize(up);

	CameraRayEmitterGroup *group;
	manta::SimpleLens lens;
	lens.initialize();
	lens.setPosition(cameraPos);
	lens.setDirection(dir);
	lens.setUp(up);
	lens.setRadius(1.0f);
	lens.setSensorResolutionX(resolutionX);
	lens.setSensorResolutionY(resolutionY);
	lens.setSensorHeight(72.0f);
	lens.setSensorWidth(72.0f * (resolutionX / (math::real)resolutionY));
	lens.update();

	RandomSampler randomSampler;

	if (!LENS_SIMULATION) {
		StandardCameraRayEmitterGroup *camera = new StandardCameraRayEmitterGroup;
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
		math::real lensHeight = 1.0f;
		math::real focusDistance = 11.0f;

		Aperture *aperture = lens.getAperture();
		aperture->setRadius((math::real)0.05);
		lens.setFocus(focusDistance);

		LensCameraRayEmitterGroup *camera = new LensCameraRayEmitterGroup;
		camera->setSampler(&randomSampler);
		camera->setDirection(math::normalize(math::sub(target, cameraPos)));
		camera->setPosition(cameraPos);
		camera->setLens(&lens);
		camera->setResolutionX(resolutionX);
		camera->setResolutionY(resolutionY);
		camera->setSampleCount(samplesPerPixel);

		group = camera;
	}

	// Create the raytracer
	rayTracer.initialize(200 * MB, 100 * MB, 12, 100, true);
	rayTracer.setBackgroundColor(getColor(255, 255, 255));
	rayTracer.setDeterministicSeedMode(DETERMINISTIC_SEED_MODE);
	rayTracer.setPathRecordingOutputDirectory("../../workspace/diagnostics/");

	// Output the results to a scene buffer
	ImagePlane sceneBuffer;

	if (TRACE_SINGLE_PIXEL) {
		rayTracer.tracePixel(616, 1459, &scene, group, &sceneBuffer);
	}
	else {
		rayTracer.traceAll(&scene, group, &sceneBuffer);
	}

	// Clean up the camera
	delete group;

	std::string fname = createUniqueRenderFilename("complex_room_demo", samplesPerPixel);
	std::string imageFname = std::string(RENDER_OUTPUT) + "bitmap/" + fname + ".jpg";
	std::string rawFname = std::string(RENDER_OUTPUT) + "raw/" + fname + ".fpm";

	RawFile rawFile;
	rawFile.writeRawFile(rawFname.c_str(), &sceneBuffer);

	writeJpeg(imageFname.c_str(), &sceneBuffer, 95);

	// Free leftover objects
	sceneBuffer.destroy();
	rayTracer.destroy();

	roomGeometry.destroy();
	kdtree.destroy();

	std::cout << "Standard allocator memory leaks:     " << StandardAllocator::Global()->getLedger() << ", " << StandardAllocator::Global()->getCurrentUsage() << std::endl;
}
