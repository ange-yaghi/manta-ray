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

	PhongDistribution phongGlassTemp;
	phongGlassTemp.setPower(10000);

	DielectricMediaInterface fresnel;
	fresnel.setIorIncident((math::real)1.0);
	fresnel.setIorTransmitted((math::real)1.0);
	MicrofacetTransmissionBSDF simpleGlassBSDF;
	simpleGlassBSDF.setDistribution(&phongGlassTemp);
	simpleGlassBSDF.setMediaInterface(&fresnel);

	SimpleBSDFMaterial *tempGlassMaterial = rayTracer.getMaterialManager()->newMaterial<SimpleBSDFMaterial>();
	tempGlassMaterial->setName("Block");
	tempGlassMaterial->setReflectance(getColor(255, 255, 255));
	tempGlassMaterial->setBSDF(&simpleGlassBSDF);

	PhongDistribution blockCoating;
	blockCoating.setPower((math::real)16000);
	BilayerBSDF blockBSDF;
	blockBSDF.setCoatingDistribution(&blockCoating);
	blockBSDF.setDiffuseMaterial(&lambert);
	blockBSDF.setDiffuse(getColor(0xf1, 0xc4, 0x0f));
	blockBSDF.setSpecularAtNormal(math::loadVector(0.1, 0.1, 0.1));
	SimpleBSDFMaterial *blockMaterial = rayTracer.getMaterialManager()->newMaterial<SimpleBSDFMaterial>();
	blockMaterial->setName("Block1");
	blockMaterial->setBSDF(&blockBSDF);

	SimpleBSDFMaterial outdoorTopLightMaterial;
	outdoorTopLightMaterial.setEmission(math::loadVector(5, 5, 5));
	outdoorTopLightMaterial.setReflectance(math::constants::Zero);

	SimpleBSDFMaterial *groundMaterial = rayTracer.getMaterialManager()->newMaterial<SimpleBSDFMaterial>();
	groundMaterial->setName("Ground");
	groundMaterial->setBSDF(&lambert);

	// Create all scene geometry
	Mesh boxCity;
	boxCity.loadObjFileData(&boxCityObj, rayTracer.getMaterialManager());
	boxCity.setFastIntersectEnabled(false);

	SpherePrimitive outdoorTopLightGeometry;
	outdoorTopLightGeometry.setRadius((math::real)10.0);
	outdoorTopLightGeometry.setPosition(math::loadVector(20, 30.0, -13.5));

	// Create scene objects
	KDTree kdtree;
	kdtree.initialize(100.0, math::constants::Zero);
	kdtree.analyze(&boxCity, 4);

	if (WRITE_KDTREE_TO_FILE) {
		kdtree.writeToObjFile("../../workspace/test_results/box_city_kdtree.obj");
	}

	SceneObject *boxCityObject = scene.createSceneObject();
	if (USE_ACCELERATION_STRUCTURE) boxCityObject->setGeometry(&kdtree);
	else boxCityObject->setGeometry(&boxCity);
	boxCityObject->setDefaultMaterial(tempGlassMaterial);

	SceneObject *outdoorTopLightObject = scene.createSceneObject();
	outdoorTopLightObject->setGeometry(&outdoorTopLightGeometry);
	outdoorTopLightObject->setDefaultMaterial(&outdoorTopLightMaterial);

	math::Vector cameraPos = math::loadVector(15.4473, 4.59977, 13.2961);
	math::Vector target = math::loadVector(2.63987, 3.55547, 2.42282);

	// Create the camera
	CameraRayEmitterGroup *group;

	math::Vector up = math::loadVector(0.0f, 1.0, 0.0);
	math::Vector dir = math::normalize(math::sub(target, cameraPos));
	up = math::cross(math::cross(dir, up), dir);
	up = math::normalize(up);

	manta::SimpleLens lens;
	lens.initialize();
	lens.setPosition(cameraPos);
	lens.setDirection(dir);
	lens.setUp(up);
	lens.setRadius(1.0);
	lens.setSensorResolutionX(resolutionX);
	lens.setSensorResolutionY(resolutionY);
	lens.setSensorHeight(10.0);
	lens.setSensorWidth(10.0 * (resolutionX / (math::real)resolutionY));
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
		math::real lensHeight = 1.0;
		math::real focusDistance = 22.0;

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

	// Initialize and run the ray tracer
	rayTracer.initialize(1000 * MB, 50 * MB, 12, 10000, true);
	rayTracer.setBackgroundColor(getColor(255, 255, 255));
	rayTracer.setDeterministicSeedMode(DETERMINISTIC_SEED_MODE);
	
	if (TRACE_SINGLE_PIXEL) {
		rayTracer.tracePixel(779, 942, &scene, group);
	}
	else {
		rayTracer.traceAll(&scene, group);
	}

	// Output the results to a scene buffer
	SceneBuffer sceneBuffer;
	group->fillSceneBuffer(&sceneBuffer);

	// Clean up the camera
	group->destroyRays();
	group->destroyEmitters();
	delete group;

	std::string fname = createUniqueRenderFilename("box_city_demo", samplesPerPixel);
	std::string imageFname = std::string(RENDER_OUTPUT) + "bitmap/" + fname + ".bmp";
	std::string rawFname = std::string(RENDER_OUTPUT) + "raw/" + fname + ".fpm";

	RawFile rawFile;
	rawFile.writeRawFile(rawFname.c_str(), &sceneBuffer);

	sceneBuffer.applyGammaCurve((math::real)(1.0 / 2.2));
	manta::SaveImageData(sceneBuffer.getBuffer(), sceneBuffer.getWidth(), sceneBuffer.getHeight(), imageFname.c_str());

	sceneBuffer.destroy();
	rayTracer.destroy();

	boxCity.destroy();
	boxCityObj.destroy();
	kdtree.destroy();

	std::cout << "Standard allocator memory leaks:     " << StandardAllocator::Global()->getLedger() << ", " << StandardAllocator::Global()->getCurrentUsage() << std::endl;
}
