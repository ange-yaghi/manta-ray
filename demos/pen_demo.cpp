#include <demos.h>

#include <iostream>

using namespace manta;

void manta_demo::penDemo(int samplesPerPixel, int resolutionX, int resolutionY) {
	// Top-level parameters
	constexpr bool LENS_SIMULATION = true;
	constexpr bool USE_ACCELERATION_STRUCTURE = true;
	constexpr bool DETERMINISTIC_SEED_MODE = false;
	constexpr bool TRACE_SINGLE_PIXEL = false;
	constexpr bool ENABLE_FRAUNHOFER_DIFFRACTION = true;

	RayTracer rayTracer;
	Scene scene;

	// Load all textures
	TextureNode texture;
	texture.loadFile(TEXTURE_PATH "/dark_wood.jpg", true);
	texture.initialize();
	texture.evaluate();

	TextureNode woodRoughness;
	woodRoughness.loadFile(TEXTURE_PATH "/wood_roughness.jpg", false);
	woodRoughness.initialize();
	woodRoughness.evaluate();

	TextureNode chromeRoughness;
	chromeRoughness.loadFile(TEXTURE_PATH "/chrome_roughness.jpg", false);
	chromeRoughness.initialize();
	chromeRoughness.evaluate();

	TextureNode floorWood;
	floorWood.loadFile(TEXTURE_PATH "/light_wood.jpg", true);
	floorWood.initialize();
	floorWood.evaluate();

	// Load all object files
	ObjFileLoader penObj;
	bool result = penObj.readObjFile(MODEL_PATH "pen.obj");

	if (!result) {
		std::cout << "Could not open geometry file(s)" << std::endl;
		penObj.destroy();
		return;
	}
	
	// Create all materials
	LambertianBSDF lambert;

	PhongDistribution chromeCoating;
	chromeCoating.setPower(10000.f);
	chromeCoating.setPowerNode(&chromeRoughness);
	chromeCoating.setMinMapPower((math::real)400.0);

	PhongDistribution woodCoating;
	woodCoating.setPower(1000.f);
	woodCoating.setPowerNode(&woodRoughness);
	woodCoating.setMinMapPower(2.f);

	PhongDistribution floorDistribution;
	floorDistribution.setPower(128.f);

	BilayerBSDF paintBsdf;
	paintBsdf.setDiffuseMaterial(&lambert);
	paintBsdf.setCoatingDistribution(&woodCoating);
	paintBsdf.setDiffuseNode(&texture);
	paintBsdf.setSpecularAtNormal(math::loadVector(0.1f, 0.1f, 0.1f));

	BilayerBSDF chromeBSDF;
	chromeBSDF.setDiffuseMaterial(&lambert);
	chromeBSDF.setCoatingDistribution(&chromeCoating);
	chromeBSDF.setDiffuse(getColor(0, 0, 0));
	chromeBSDF.setSpecularAtNormal(math::loadVector(0.95f, 0.95f, 0.95f));

	MicrofacetReflectionBSDF floorBSDF;
	floorBSDF.setDistribution(&floorDistribution);

	SimpleBSDFMaterial *paintMaterial = rayTracer.getMaterialManager()->newMaterial<SimpleBSDFMaterial>();
	paintMaterial->setBSDF(&paintBsdf);
	paintMaterial->setName("PenBody");

	SimpleBSDFMaterial *chromeMaterial = rayTracer.getMaterialManager()->newMaterial<SimpleBSDFMaterial>();
	chromeMaterial->setBSDF(&chromeBSDF);
	chromeMaterial->setName("Chrome");

	SimpleBSDFMaterial *floorMaterial = rayTracer.getMaterialManager()->newMaterial<SimpleBSDFMaterial>();
	floorMaterial->setBSDF(&floorBSDF);
	floorMaterial->setReflectanceNode(&floorWood);
	floorMaterial->setName("Backdrop");

	SimpleBSDFMaterial *strongLight = rayTracer.getMaterialManager()->newMaterial<SimpleBSDFMaterial>();
	strongLight->setEmission(math::loadVector(2.0f, 2.0f, 2.0f));
	strongLight->setReflectance(math::constants::Zero);
	strongLight->setName("StrongLight");

	SimpleBSDFMaterial *weakLight = rayTracer.getMaterialManager()->newMaterial<SimpleBSDFMaterial>();
	weakLight->setEmission(math::loadVector(1.0f, 1.0f, 1.0f));
	weakLight->setReflectance(math::constants::Zero);
	weakLight->setName("WeakLight");

	// Create all scene geometry
	Mesh pen;
	pen.loadObjFileData(&penObj, rayTracer.getMaterialManager(), chromeMaterial->getIndex(), 0);

	// Destroy file loaders
	penObj.destroy();

	KDTree kdtree;
	kdtree.initialize(150.f, math::constants::Zero);
	kdtree.analyze(&pen, 4);

	// Create scene objects
	SceneObject *penObject = scene.createSceneObject();
	if (USE_ACCELERATION_STRUCTURE) {
		penObject->setGeometry(&kdtree);
	}
	else {
		penObject->setGeometry(&pen);
	}
	penObject->setDefaultMaterial(chromeMaterial);
	penObject->setName("Pen");

	// Create the camera
	math::Vector cameraPos = math::loadVector(9.436f, 1.2f, 4.5370f);
	math::Vector target = math::loadVector(1.3f, 0.35547f, 0.0f);

	math::Vector up = math::loadVector(0.0f, 1.0f, 0.0f);
	math::Vector dir = math::normalize(math::sub(target, cameraPos));
	up = math::cross(math::cross(dir, up), dir);
	up = math::normalize(up);

	cameraPos = math::sub(cameraPos, math::mul(dir, math::loadScalar(3.9f)));

	CameraRayEmitterGroup *group;
	manta::SimpleLens lens;
	manta::PolygonalAperture polygonalAperture;
	polygonalAperture.initialize(6);

	lens.setAperture(&polygonalAperture);
	lens.initialize();
	lens.setPosition(cameraPos);
	lens.setDirection(dir);
	lens.setUp(up);
	lens.setRadius(1.0f);
	lens.setSensorResolutionX(resolutionX);
	lens.setSensorResolutionY(resolutionY);
	lens.setSensorHeight(22.0f);
	lens.setSensorWidth(22.0f * (resolutionX / (math::real)resolutionY));
	lens.update();

	RandomSampler sampler;
	SimpleSampler simpleSampler;

	if (!LENS_SIMULATION) {
		StandardCameraRayEmitterGroup *camera = new StandardCameraRayEmitterGroup;
		camera->setSampler(&sampler);
		camera->setDirection(dir);
		camera->setPosition(cameraPos);
		camera->setUp(up);
		camera->setPlaneDistance(1.0f);
		camera->setPlaneHeight(0.25f);
		camera->setResolutionX(resolutionX);
		camera->setResolutionY(resolutionY);
		camera->setSampleCount(samplesPerPixel);
		group = camera;
	}
	else {
		math::real lensHeight = 1.0f;
		math::real focusDistance = 11.0f;

		Aperture *aperture = lens.getAperture();
		aperture->setRadius((math::real)0.007); // 0.007
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

	// Create the raytracer
	rayTracer.initialize(200 * MB, 5 * MB, 12, 100, true);
	rayTracer.setBackgroundColor(getColor(0, 0, 0));
	rayTracer.setDeterministicSeedMode(DETERMINISTIC_SEED_MODE);
	rayTracer.setPathRecordingOutputDirectory("../../workspace/diagnostics/");

	// Output the results to a scene buffer
	ImagePlane sceneBuffer;

	if (TRACE_SINGLE_PIXEL) {
		rayTracer.tracePixel(179, 1423, &scene, group, &sceneBuffer);
	}
	else {
		rayTracer.traceAll(&scene, group, &sceneBuffer);
	}

	// Clean up the camera
	delete group;

	std::string fname = createUniqueRenderFilename("pen_demo", samplesPerPixel);
	std::string imageFname = std::string(RENDER_OUTPUT) + "bitmap/" + fname + ".jpg";
	std::string fraunFname = std::string(RENDER_OUTPUT) + "bitmap/" + fname + "_fraun" + ".jpg";
	std::string convFname = std::string(RENDER_OUTPUT) + "bitmap/" + fname + "_conv" + ".jpg";
	std::string rawFname = std::string(RENDER_OUTPUT) + "raw/" + fname + ".fpm";

	if (ENABLE_FRAUNHOFER_DIFFRACTION) {
		VectorMap2D base;
		base.copy(&sceneBuffer);

		int safeWidth = base.getSafeWidth();

		FraunhoferDiffraction testFraun;
		FraunhoferDiffraction::Settings settings;
		FraunhoferDiffraction::defaultSettings(&settings);
		settings.frequencyMultiplier = 1.0;
		settings.maxSamples = 4096;
		settings.textureSamples = 10;

		TextureNode dirtTexture;
		dirtTexture.loadFile(TEXTURE_PATH "dirt_very_soft.png", true);
		dirtTexture.initialize();
		dirtTexture.evaluate();

		CmfTable colorTable;
		Spectrum sourceSpectrum;
		colorTable.loadCsv(CMF_PATH "xyz_cmf_31.csv");
		sourceSpectrum.loadCsv(CMF_PATH "d65_lighting.csv");

		lens.getAperture()->setRadius((math::real)0.18);
		testFraun.generate(lens.getAperture(), &dirtTexture, safeWidth, 8.0f, &colorTable, &sourceSpectrum, &settings);

		VectorMapWrapperNode fraunNode(testFraun.getDiffractionPattern());
		fraunNode.initialize();
		fraunNode.evaluate();

		ImageOutputNode fraunOutputNode;
		fraunOutputNode.setJpegQuality(95);
		fraunOutputNode.setGammaCorrection(true);
		fraunOutputNode.setOutputFilename(fraunFname);
		fraunOutputNode.setInputNode(&fraunNode);
		fraunOutputNode.initialize();
		fraunOutputNode.evaluate();
		fraunOutputNode.destroy();

		VectorMapWrapperNode baseNode(&base);
		baseNode.initialize();
		baseNode.evaluate();
		baseNode.destroy();

		ConvolutionNode convNode;
		convNode.setInputs(&baseNode, &fraunNode);
		convNode.setResize(true);
		convNode.setClip(true);
		convNode.initialize();
		convNode.evaluate();

		testFraun.destroy();
		base.destroy();

		ImageOutputNode outputNode;
		outputNode.setJpegQuality(95);
		outputNode.setGammaCorrection(true);
		outputNode.setOutputFilename(convFname);
		outputNode.setInputNode(&convNode);
		outputNode.initialize();
		outputNode.evaluate();
		outputNode.destroy();

		colorTable.destroy();
		sourceSpectrum.destroy();
		convNode.destroy();
		dirtTexture.destroy();
	}

	RawFile rawFile;
	rawFile.writeRawFile(rawFname.c_str(), &sceneBuffer);

	writeJpeg(imageFname.c_str(), &sceneBuffer, 95);

	sceneBuffer.destroy();
	polygonalAperture.destroy();
	texture.destroy();
	woodRoughness.destroy();
	chromeRoughness.destroy();
	floorWood.destroy();
	rayTracer.destroy();
	pen.destroy();
	kdtree.destroy();

	std::cout << "Standard allocator memory leaks:     " << StandardAllocator::Global()->getLedger() << ", " << StandardAllocator::Global()->getCurrentUsage() << std::endl;
}
