#include <demos.h>

using namespace manta;

void manta_demo::boxCityDemo(int samplesPerPixel, int resolutionX, int resolutionY) {
	// Top-level parameters
	constexpr bool USE_ACCELERATION_STRUCTURE = true;
	constexpr bool DETERMINISTIC_SEED_MODE = false;
	constexpr bool TRACE_SINGLE_PIXEL = false;
	constexpr bool WRITE_KDTREE_TO_FILE = false;
	constexpr bool LENS_SIMULATION = false;

	Scene scene;
	RayTracer rayTracer;

	ObjFileLoader boxCityObj;
	bool result = boxCityObj.readObjFile(MODEL_PATH "box_city.obj");

	if (!result) {
		std::cout << "Could not open geometry file" << std::endl;

		boxCityObj.destroy();

		return;
	}

	// Create all materials
	LambertianBSDF lambert;

	PhongDistribution blockCoating;
	blockCoating.setPower((math::real)300);
	BilayerBSDF blockBSDF;
	blockBSDF.setCoatingDistribution(&blockCoating);
	blockBSDF.setDiffuseMaterial(&lambert);
	blockBSDF.setDiffuse(getColor(0xf1, 0xc4, 0x0f));
	blockBSDF.setSpecularAtNormal(math::loadVector(0.02f, 0.02f, 0.02f));
	SimpleBSDFMaterial *blockMaterial = rayTracer.getMaterialManager()->newMaterial<SimpleBSDFMaterial>();
	blockMaterial->setName("Block");
	blockMaterial->setBSDF(&blockBSDF);

	SimpleBSDFMaterial outdoorTopLightMaterial;
	outdoorTopLightMaterial.setEmission(math::loadVector(5.f, 5.f, 5.f));
	outdoorTopLightMaterial.setReflectance(math::constants::Zero);

	SimpleBSDFMaterial *groundMaterial = rayTracer.getMaterialManager()->newMaterial<SimpleBSDFMaterial>();
	groundMaterial->setName("Ground");
	groundMaterial->setBSDF(&lambert);

	// Create all scene geometry
	Mesh boxCity;
	boxCity.loadObjFileData(&boxCityObj, rayTracer.getMaterialManager());
	boxCity.setFastIntersectEnabled(false);
	//boxCity.findQuads();

	SpherePrimitive outdoorTopLightGeometry;
	outdoorTopLightGeometry.setRadius((math::real)10.0);
	outdoorTopLightGeometry.setPosition(math::loadVector(20.f, 30.0f, -13.5f));

	// Create scene objects
	KDTree kdtree;
	kdtree.initialize(100.0f, math::constants::Zero);
	kdtree.analyze(&boxCity, 2);

	if (WRITE_KDTREE_TO_FILE) {
		kdtree.writeToObjFile("../../workspace/test_results/box_city_kdtree.obj");
	}

	SceneObject *boxCityObject = scene.createSceneObject();
	if (USE_ACCELERATION_STRUCTURE) boxCityObject->setGeometry(&kdtree);
	else boxCityObject->setGeometry(&boxCity);
	boxCityObject->setDefaultMaterial(blockMaterial);

	SceneObject *outdoorTopLightObject = scene.createSceneObject();
	outdoorTopLightObject->setGeometry(&outdoorTopLightGeometry);
	outdoorTopLightObject->setDefaultMaterial(&outdoorTopLightMaterial);

	math::Vector cameraPos = math::loadVector(15.4473f, 4.59977f, 13.2961f);
	math::Vector target = math::loadVector(2.63987f, 3.55547f, 2.42282f);

	// Create the camera
	CameraRayEmitterGroup *group;

	math::Vector up = math::loadVector(0.0f, 1.0f, 0.0f);
	math::Vector dir = math::normalize(math::sub(target, cameraPos));
	up = math::cross(math::cross(dir, up), dir);
	up = math::normalize(up);

	manta::SimpleLens lens;
	lens.initialize();
	lens.setPosition(cameraPos);
	lens.setDirection(dir);
	lens.setUp(up);
	lens.setRadius(1.0f);
	lens.setSensorResolutionX(resolutionX);
	lens.setSensorResolutionY(resolutionY);
	lens.setSensorHeight(10.0f);
	lens.setSensorWidth(10.0f * (resolutionX / (math::real)resolutionY));
	lens.update();

	RandomSampler sampler;

	if (!LENS_SIMULATION) {
		StandardCameraRayEmitterGroup *camera = new StandardCameraRayEmitterGroup;
		camera->setDirection(dir);
		camera->setPosition(cameraPos);
		camera->setUp(up);
		camera->setPlaneDistance(1.0f);
		camera->setPlaneHeight(1.0f);
		camera->setResolutionX(resolutionX);
		camera->setResolutionY(resolutionY);
		camera->setSampleCount(samplesPerPixel);
		camera->setSampler(&sampler);

		group = camera;
	}
	else {
		math::real lensHeight = 1.0f;
		math::real focusDistance = 22.0f;

		Aperture *aperture = lens.getAperture();
		aperture->setRadius((math::real)0.18);
		lens.setFocus(focusDistance);

		LensCameraRayEmitterGroup *camera = new LensCameraRayEmitterGroup;
		camera->setDirection(math::normalize(math::sub(target, cameraPos)));
		camera->setPosition(cameraPos);
		camera->setLens(&lens);
		camera->setResolutionX(resolutionX);
		camera->setResolutionY(resolutionY);
		camera->setSampleCount(samplesPerPixel);
		camera->setSampler(&sampler);

		group = camera;
	}

	// Output the results to a scene buffer
	SceneBuffer sceneBuffer;

	// Initialize and run the ray tracer
	rayTracer.initialize(200 * MB, 50 * MB, 12, 100, true);
	rayTracer.setBackgroundColor(getColor(255, 255, 255));
	rayTracer.setDeterministicSeedMode(DETERMINISTIC_SEED_MODE);
	
	if (TRACE_SINGLE_PIXEL) {
		rayTracer.tracePixel(779, 942, &scene, group, &sceneBuffer);
	}
	else {
		rayTracer.traceAll(&scene, group, &sceneBuffer);
	}

	// Clean up the camera
	delete group;

	std::string fname = createUniqueRenderFilename("box_city_demo", samplesPerPixel);
	std::string imageFname = std::string(RENDER_OUTPUT) + "bitmap/" + fname + ".jpg";
	std::string rawFname = std::string(RENDER_OUTPUT) + "raw/" + fname + ".fpm";

	RawFile rawFile;
	rawFile.writeRawFile(rawFname.c_str(), &sceneBuffer);

	sceneBuffer.applyGammaCurve((math::real)(1.0 / 2.2));
	writeJpeg(imageFname.c_str(), &sceneBuffer, 95);

	sceneBuffer.destroy();
	rayTracer.destroy();

	boxCity.destroy();
	boxCityObj.destroy();
	kdtree.destroy();

	std::cout << "Standard allocator memory leaks:     " << StandardAllocator::Global()->getLedger() << ", " << StandardAllocator::Global()->getCurrentUsage() << std::endl;
}
