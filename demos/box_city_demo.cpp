#include <demos.h>

using namespace manta;

void manta_demo::boxCityDemo(int samplesPerPixel, int resolutionX, int resolutionY) {
	// Top-level parameters
	constexpr bool USE_ACCELERATION_STRUCTURE = true;
	constexpr bool DETERMINISTIC_SEED_MODE = false;
	constexpr bool TRACE_SINGLE_PIXEL = false;
	constexpr bool WRITE_KDTREE_TO_FILE = false;
	constexpr bool LENS_SIMULATION = false;
	constexpr bool POLYGON_APERTURE = true;
	constexpr bool ENABLE_FRAUNHOFER_DIFFRACTION = false;

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
	blockBSDF.setCoatingDistribution(blockCoating.getMainOutput());
	blockBSDF.setDiffuseMaterial(&lambert);
	blockBSDF.setDiffuse(getColor(0xf1, 0xc4, 0x0f)); // 0xf1, 0xc4, 0x0f
	blockBSDF.setSpecularAtNormal(math::loadVector(0.02f, 0.02f, 0.02f));
	SimpleBSDFMaterial *blockMaterial = rayTracer.getMaterialManager()->newMaterial<SimpleBSDFMaterial>();
	blockMaterial->setName("Block");
	blockMaterial->setBSDF(&blockBSDF);
	//blockMaterial->setReflectance(math::loadVector(0.01f, 0.01f, 0.01f));

	SimpleBSDFMaterial outdoorTopLightMaterial;
	outdoorTopLightMaterial.setEmission(math::loadVector(1.f, 1.f, 1.f));
	outdoorTopLightMaterial.setReflectance(math::constants::Zero);

	SimpleBSDFMaterial *groundMaterial = rayTracer.getMaterialManager()->newMaterial<SimpleBSDFMaterial>();
	groundMaterial->setName("Ground");
	//groundMaterial->setReflectance(math::loadVector(0.01f, 0.01f, 0.01f));
	groundMaterial->setBSDF(&lambert);

	SimpleBSDFMaterial *sunMaterial = rayTracer.getMaterialManager()->newMaterial<SimpleBSDFMaterial>();
	sunMaterial->setName("Sun");
	sunMaterial->setReflectance(math::loadVector(0.0f, 0.0f, 0.0f));
	sunMaterial->setEmission(math::loadVector(1000.0f, 1000.0f, 1000.0f));
	sunMaterial->setBSDF(nullptr);

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
	kdtree.initialize(1000.0f, math::constants::Zero);
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

	// Easy camera position control
	cameraPos = math::sub(cameraPos, math::mul(dir, math::loadScalar(0.f)));

	manta::SimpleLens lens;
	manta::PolygonalAperture polygonalAperture;
	polygonalAperture.setBladeCurvature(0.4f);
	polygonalAperture.initialize(8);

	if (POLYGON_APERTURE) lens.setAperture(&polygonalAperture);
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

	std::string fname = createUniqueRenderFilename("box_city_demo", samplesPerPixel);
	std::string imageFname = std::string(RENDER_OUTPUT) + "bitmap/" + fname + ".jpg";
	std::string fraunFname = std::string(RENDER_OUTPUT) + "bitmap/" + fname + "_fraun" + ".jpg";
	std::string convFname = std::string(RENDER_OUTPUT) + "bitmap/" + fname + "_conv" + ".jpg";
	std::string rawFname = std::string(RENDER_OUTPUT) + "raw/" + fname + ".fpm";

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
	ImagePlane sceneBuffer;

	// Initialize and run the ray tracer
	rayTracer.initialize(200 * MB, 50 * MB, 12, 100, true);
	rayTracer.setBackgroundColor(getColor(0xff, 0xff, 0xff));
	rayTracer.setPathRecordingOutputDirectory("../../workspace/diagnostics/");
	rayTracer.setDeterministicSeedMode(DETERMINISTIC_SEED_MODE);
	
	if (TRACE_SINGLE_PIXEL) {
		rayTracer.tracePixel(915, 985, &scene, group, &sceneBuffer);
	}
	else {
		rayTracer.traceAll(&scene, group, &sceneBuffer);
	}

	// Clean up the camera
	delete group;

	RawFile rawFile;
	rawFile.writeRawFile(rawFname.c_str(), &sceneBuffer);

	if (ENABLE_FRAUNHOFER_DIFFRACTION) {
		VectorMap2D base;
		base.copy(&sceneBuffer);

		int safeWidth = base.getSafeWidth();

		FraunhoferDiffraction testFraun;
		FraunhoferDiffraction::Settings settings;
		FraunhoferDiffraction::setDefaultSettings(&settings);
		settings.frequencyMultiplier = 1.0;
		settings.maxSamples = 4096;
		settings.textureSamples = 10;

		TextureNode dirtTexture;
		dirtTexture.loadFile(TEXTURE_PATH "dirt_very_soft.png", true);
		dirtTexture.initialize();
		dirtTexture.evaluate();
		const VectorMap2D *dirtTextureMap = dirtTexture.getMainOutput()->getMap();

		CmfTable colorTable;
		Spectrum sourceSpectrum;
		colorTable.loadCsv(CMF_PATH "xyz_cmf_31.csv");
		sourceSpectrum.loadCsv(CMF_PATH "d65_lighting.csv");

		testFraun.generate(&polygonalAperture, dirtTextureMap, safeWidth, 5.0f, &colorTable, &sourceSpectrum, &settings);

		VectorMapWrapperNode fraunNode(testFraun.getDiffractionPattern());
		fraunNode.initialize();
		fraunNode.evaluate();

		ImageOutputNode fraunOutputNode;
		fraunOutputNode.setJpegQuality(95);
		fraunOutputNode.setGammaCorrection(true);
		fraunOutputNode.setOutputFilename(fraunFname);
		fraunOutputNode.setInput(fraunNode.getMainOutput());
		fraunOutputNode.initialize();
		fraunOutputNode.evaluate();
		fraunOutputNode.destroy();

		VectorMapWrapperNode mantaOutput(&base);
		mantaOutput.initialize();
		mantaOutput.evaluate();
		mantaOutput.destroy();

		RampNode ramp;
		ramp.initialize();
		ramp.getMainOutput()->setDefaultDc(math::constants::One);
		ramp.getMainOutput()->setDefaultFoot(math::constants::One);
		ramp.getMainOutput()->setDefaultSlope(math::loadScalar(10.0f));
		ramp.getMainOutput()->setInput(mantaOutput.getMainOutput());
		ramp.evaluate();

		MultiplyNode mulNode;
		mulNode.initialize();
		mulNode.getMainOutput()->setInputA(ramp.getMainOutput());
		mulNode.getMainOutput()->setInputB(mantaOutput.getMainOutput());
		mulNode.evaluate();

		ConvolutionNode convNode;
		convNode.initialize();
		convNode.setInputs(mulNode.getMainOutput(), fraunNode.getMainOutput());
		convNode.setResize(true);
		convNode.setClip(true);
		convNode.evaluate();

		testFraun.destroy();
		base.destroy();

		ImageOutputNode outputNode;
		outputNode.setJpegQuality(95);
		outputNode.setGammaCorrection(true);
		outputNode.setOutputFilename(convFname);
		outputNode.setInput(convNode.getMainOutput());
		outputNode.initialize();
		outputNode.evaluate();
		outputNode.destroy();

		colorTable.destroy();
		sourceSpectrum.destroy();
		convNode.destroy();
		dirtTexture.destroy();
	}

	writeJpeg(imageFname.c_str(), &sceneBuffer, 95);

	sceneBuffer.destroy();
	rayTracer.destroy();

	boxCity.destroy();
	boxCityObj.destroy();
	kdtree.destroy();
	polygonalAperture.destroy();

	std::cout << "Standard allocator memory leaks:     " << StandardAllocator::Global()->getLedger() << ", " << StandardAllocator::Global()->getCurrentUsage() << std::endl;
}
