#include <demos.h>

#include <iostream>

using namespace manta;

void manta_demo::teapotLampDemo(int samplesPerPixel, int resolutionX, int resolutionY) {
	enum OBJECT {
		TEAPOT,
		SPHERE
	};

	enum MATERIAL {
		ENAMEL,
		GLASS
	};

	// Top-level parameters
	constexpr bool BLOCK_ALL_LIGHT = false;
	constexpr bool USE_ACCELERATION_STRUCTURE = true;
	constexpr bool DETERMINISTIC_SEED_MODE = false;
	constexpr bool TRACE_SINGLE_PIXEL = false;
	constexpr OBJECT OBJECT = TEAPOT;
	constexpr MATERIAL MATERIAL = ENAMEL;
	constexpr bool ENABLE_FRAUNHOFER_DIFFRACTION = true;

	Scene scene;

	// Load all object files
	ObjFileLoader lampObj;
	bool lampObjResult = lampObj.loadObjFile(MODEL_PATH "lamp2.obj");

	ObjFileLoader teapotObj;
	bool teapotObjResult;
	
	if (OBJECT == TEAPOT) {
		teapotObjResult = teapotObj.loadObjFile(MODEL_PATH "lamp_teapot.obj");
	}
	else if (OBJECT == SPHERE) {
		teapotObjResult = teapotObj.loadObjFile(MODEL_PATH "sphere_lamp.obj");
	}

	ObjFileLoader lampBlockObj;
	bool lampBlockObjResult = lampBlockObj.loadObjFile(MODEL_PATH "lampblock.obj");

	if (!lampObjResult || !teapotObjResult || !lampBlockObjResult) {
		std::cout << "Could not open geometry file" << std::endl;

		lampObj.destroy();
		teapotObj.destroy();
		lampBlockObj.destroy();

		return;
	}

	RayTracer rayTracer;
	rayTracer.setMaterialManager(new MaterialManager);

	// Create all materials
	LambertianBSDF lambert;

	PhongDistribution floorCoating;
	floorCoating.setPower((math::real)128);

	BilayerBSDF floorBSDF;
	floorBSDF.setCoatingDistribution(floorCoating.getMainOutput());
	floorBSDF.setDiffuse(getColor(0xFF, 0xFF, 0xFF));
	floorBSDF.setDiffuseMaterial(&lambert);
	floorBSDF.setSpecularAtNormal(math::loadVector(0.0, 0.0, 0.0));

	SimpleBSDFMaterial *floorMaterial = rayTracer.getMaterialManager()->newMaterial<SimpleBSDFMaterial>();
	floorMaterial->setBSDF(&floorBSDF);

	SimpleBSDFMaterial *lampLightMaterial = rayTracer.getMaterialManager()->newMaterial<SimpleBSDFMaterial>();
	lampLightMaterial->setEmission(math::mul(getColor(255, 197, 143), math::loadScalar(50.0)));
	lampLightMaterial->setReflectance(math::constants::Zero);

	PhongDistribution teapotCoating;
	teapotCoating.setPower((math::real)1024);

	BilayerBSDF teapotBSDF;
	teapotBSDF.setCoatingDistribution(teapotCoating.getMainOutput());
	teapotBSDF.setDiffuse(getColor(0x89, 0xCF, 0xF0));
	teapotBSDF.setDiffuseMaterial(&lambert);
	teapotBSDF.setSpecularAtNormal(math::loadVector(0.5, 0.5, 0.5));

	SimpleBSDFMaterial *teapotMaterial = rayTracer.getMaterialManager()->newMaterial<SimpleBSDFMaterial>();
	teapotMaterial->setBSDF(&teapotBSDF);

	// Glass
	PhongDistribution phongGlass;
	phongGlass.setPower(5000);
	phongGlass.setMinMapPower(800);
	
	DielectricMediaInterface fresnel;
	fresnel.setIorIncident((math::real)1.0);
	fresnel.setIorTransmitted((math::real)1.4);
	MicrofacetGlassBSDF simpleGlassBSDF;
	simpleGlassBSDF.setDistribution(&phongGlass);
	simpleGlassBSDF.setMediaInterface(&fresnel);

	SimpleBSDFMaterial *glassMaterial = rayTracer.getMaterialManager()->newMaterial<SimpleBSDFMaterial>();
	glassMaterial->setName("Glass");
	glassMaterial->setReflectance(getColor(255, 255, 255));
	glassMaterial->setBSDF(&simpleGlassBSDF);

	// Create all scene geometry
	Mesh lamp;
	lamp.loadObjFileData(&lampObj, rayTracer.getMaterialManager(), floorMaterial->getIndex());
	lamp.setFastIntersectEnabled(true);
	lamp.setFastIntersectRadius((math::real)2.123);
	lamp.setFastIntersectPosition(math::loadVector(-0.06430f, 1.86833f, -2.96564f));

	Mesh lampBlock;
	lampBlock.loadObjFileData(&lampBlockObj);
	lampBlock.setFastIntersectEnabled(false);
	lampBlock.setFastIntersectRadius((math::real)4.0);

	Material *mainObjectMaterial;
	if (MATERIAL == GLASS) mainObjectMaterial = glassMaterial;
	else if (MATERIAL == ENAMEL) mainObjectMaterial = teapotMaterial;

	Mesh objectMesh;
	objectMesh.loadObjFileData(&teapotObj, rayTracer.getMaterialManager(), mainObjectMaterial->getIndex());
	objectMesh.setFastIntersectEnabled(true);
	objectMesh.setFastIntersectRadius((math::real)2.0);
	objectMesh.setFastIntersectPosition(math::loadVector(-0.5724f, 1.02483f, -0.04969f));

	objectMesh.merge(&lamp);
	objectMesh.findQuads();

	KDTree kdtree;
	kdtree.initialize(4.0f, math::constants::Zero);
	kdtree.analyzeWithProgress(&objectMesh, 4);

	SpherePrimitive bulb;
	bulb.setRadius(0.25f);
	bulb.setPosition(math::loadVector(0.10669f, 3.42135f, -2.47464f));

	SpherePrimitive groundGeometry;
	groundGeometry.setRadius((math::real)5000.0);
	groundGeometry.setPosition(math::loadVector(0.0f, -5000.001f, 0.f));

	// Create scene objects
	SceneObject *bulbObject = scene.createSceneObject();
	bulbObject->setGeometry(&bulb);
	bulbObject->setDefaultMaterial(lampLightMaterial);
	bulbObject->setName("Bulb");

	SceneObject *teapotObject = scene.createSceneObject();
	if (USE_ACCELERATION_STRUCTURE) teapotObject->setGeometry(&kdtree);
	else teapotObject->setGeometry(&objectMesh);
	teapotObject->setDefaultMaterial(teapotMaterial);
	teapotObject->setName("MainObject");

	SceneObject *ground = scene.createSceneObject();
	ground->setGeometry(&groundGeometry);
	ground->setDefaultMaterial(floorMaterial);
	ground->setName("Ground");

	if (BLOCK_ALL_LIGHT) {
		SceneObject *lampBlockObject = scene.createSceneObject();
		lampBlockObject->setGeometry(&lampBlock);
		lampBlockObject->setDefaultMaterial(floorMaterial);
		lampBlockObject->setName("LampBlock");
	}

	// Create the camera
	RandomSampler randomSampler;
	StandardCameraRayEmitterGroup camera;
	camera.setDirection(math::loadVector(-1.0f, 0.0f, 0.0f));
	camera.setPosition(math::loadVector(7.0f, 2.0f, 0.0f));
	camera.setUp(math::loadVector(0.0f, 1.0f, 0.0f));
	camera.setPlaneDistance(1.0f);
	camera.setPlaneHeight(1.0f);
	camera.setResolutionX(resolutionX);
	camera.setResolutionY(resolutionY);
	camera.setSampleCount(samplesPerPixel);
	camera.setSampler(&randomSampler);

	// Create the raytracer
	rayTracer.initialize(200 * MB, 100 * MB, 12, 100, true);
	rayTracer.setBackgroundColor(getColor(0, 0, 0));
	rayTracer.setDeterministicSeedMode(DETERMINISTIC_SEED_MODE);
	rayTracer.setPathRecordingOutputDirectory("../../workspace/diagnostics/");

	// Output the results to a scene buffer
	ImagePlane sceneBuffer;

	if (TRACE_SINGLE_PIXEL) {
		rayTracer.tracePixel(1025, 921, &scene, &camera, &sceneBuffer);
	}
	else {
		rayTracer.traceAll(&scene, &camera, &sceneBuffer);
	}

	std::string fname = createUniqueRenderFilename("teapot_lamp_demo", samplesPerPixel);
	std::string imageFname = std::string(RENDER_OUTPUT) + "bitmap/" + fname + ".jpg";
	std::string fraunFname = std::string(RENDER_OUTPUT) + "bitmap/" + fname + "_fraun.jpg";
	std::string convFname = std::string(RENDER_OUTPUT) + "bitmap/" + fname + "_conv.jpg";
	std::string rawFname = std::string(RENDER_OUTPUT) + "raw/" + fname + ".fpm";

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
		dirtTexture.loadFile(TEXTURE_PATH "dirt_composite.png", true);
		dirtTexture.initialize();
		dirtTexture.evaluate();
		const VectorMap2D *dirtTextureMap = dirtTexture.getMainOutput()->getMap();

		CmfTable colorTable;
		Spectrum sourceSpectrum;
		colorTable.loadCsv(CMF_PATH "xyz_cmf_31.csv");
		sourceSpectrum.loadCsv(CMF_PATH "d65_lighting.csv");

		PolygonalAperture aperture;
		aperture.initialize(6);
		aperture.setRadius(0.18f);
		aperture.setBladeCurvature(0.3f);
		testFraun.generate(&aperture, dirtTextureMap, safeWidth, 16.0f, &colorTable, &sourceSpectrum, &settings);
		aperture.destroy();

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

		StepNode step;
		step.initialize();
		step.getMainOutput()->setDefaultDc(math::constants::One);
		step.getMainOutput()->setDefaultFoot(math::constants::One);
		step.getMainOutput()->setDefaultStep(math::loadScalar(100.0f));
		step.getMainOutput()->setInput(mantaOutput.getMainOutput());
		step.evaluate();

		MultiplyNode mulNode;
		mulNode.initialize();
		mulNode.getMainOutput()->setInputA(step.getMainOutput());
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

	RawFile rawFile;
	rawFile.writeRawFile(rawFname.c_str(), &sceneBuffer);

	writeJpeg(imageFname.c_str(), &sceneBuffer, 95);

	sceneBuffer.destroy();
	rayTracer.destroy();

	lamp.destroy();
	lampBlock.destroy();
	lampBlockObj.destroy();
	lampObj.destroy();
	objectMesh.destroy();
	teapotObj.destroy();
	kdtree.destroy();

	std::cout << "Standard allocator memory leaks:     " << StandardAllocator::Global()->getLedger() << ", " << StandardAllocator::Global()->getCurrentUsage() << std::endl;
}
