#include <demos.h>

#include <iostream>

using namespace manta;

void manta_demo::teapotLampDemo(int samplesPerPixel, int resolutionX, int resolutionY) {
	enum OBJECT {
		TEAPOT,
		SPHERE
	};

	// Top-level parameters
	constexpr bool BLOCK_ALL_LIGHT = false;
	constexpr bool USE_ACCELERATION_STRUCTURE = true;
	constexpr bool DETERMINISTIC_SEED_MODE = false;
	constexpr bool TRACE_SINGLE_PIXEL = false;
	constexpr OBJECT OBJECT = TEAPOT;

	Scene scene;

	// Load all object files
	ObjFileLoader lampObj;
	bool lampObjResult = lampObj.readObjFile(MODEL_PATH "lamp2.obj");

	ObjFileLoader teapotObj;
	bool teapotObjResult;
	
	if (OBJECT == TEAPOT) {
		teapotObjResult = teapotObj.readObjFile(MODEL_PATH "teapot_lamp.obj");
	}
	else if (OBJECT == SPHERE) {
		teapotObjResult = teapotObj.readObjFile(MODEL_PATH "sphere_lamp.obj");
	}

	ObjFileLoader lampBlockObj;
	bool lampBlockObjResult = lampBlockObj.readObjFile(MODEL_PATH "lampblock.obj");

	if (!lampObjResult || !teapotObjResult || !lampBlockObjResult) {
		std::cout << "Could not open geometry file" << std::endl;

		lampObj.destroy();
		teapotObj.destroy();
		lampBlockObj.destroy();

		return;
	}

	RayTracer rayTracer;

	// Create all materials
	LambertianBSDF lambert;

	PhongDistribution floorCoating;
	floorCoating.setPower((math::real)128);

	BilayerBSDF floorBSDF;
	floorBSDF.setCoatingDistribution(&floorCoating);
	floorBSDF.setDiffuse(getColor(0xFF, 0xFF, 0xFF));
	floorBSDF.setDiffuseMaterial(&lambert);
	floorBSDF.setSpecularAtNormal(math::loadVector(0.0, 0.0, 0.0));

	SimpleBSDFMaterial *floorMaterial = rayTracer.getMaterialManager()->newMaterial<SimpleBSDFMaterial>();
	floorMaterial->setBSDF(&floorBSDF);

	SimpleBSDFMaterial *lampLightMaterial = rayTracer.getMaterialManager()->newMaterial<SimpleBSDFMaterial>();
	lampLightMaterial->setEmission(math::mul(getColor(255, 197, 143), math::loadScalar(30.0)));
	lampLightMaterial->setReflectance(math::constants::Zero);

	PhongDistribution teapotCoating;
	teapotCoating.setPower((math::real)1024);

	BilayerBSDF teapotBSDF;
	teapotBSDF.setCoatingDistribution(&teapotCoating);
	teapotBSDF.setDiffuse(getColor(0xFF, 0x08, 0x14));
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
	lamp.setFastIntersectPosition(math::loadVector(-0.06430, 1.86833, -2.96564));

	Mesh lampBlock;
	lampBlock.loadObjFileData(&lampBlockObj);
	lampBlock.setFastIntersectEnabled(false);
	lampBlock.setFastIntersectRadius((math::real)4.0);

	Material *mainObjectMaterial;
	if (OBJECT == TEAPOT) mainObjectMaterial = teapotMaterial;
	else if (OBJECT == SPHERE) mainObjectMaterial = glassMaterial;

	Mesh objectMesh;
	objectMesh.loadObjFileData(&teapotObj, rayTracer.getMaterialManager(), glassMaterial->getIndex());
	objectMesh.setFastIntersectEnabled(true);
	objectMesh.setFastIntersectRadius((math::real)2.0);
	objectMesh.setFastIntersectPosition(math::loadVector(-0.5724, 1.02483, -0.04969));

	objectMesh.merge(&lamp);
	objectMesh.findQuads();

	KDTree kdtree;
	kdtree.initialize(4.0, math::constants::Zero);
	kdtree.analyze(&objectMesh, 4);

	SpherePrimitive bulb;
	bulb.setRadius(0.25);
	bulb.setPosition(math::loadVector(0.10669, 3.42135, -2.47464));

	SpherePrimitive groundGeometry;
	groundGeometry.setRadius((math::real)5000.0);
	groundGeometry.setPosition(math::loadVector(0.0, -5000.001, 0));

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
	camera.setDirection(math::loadVector(-1.0, 0.0, 0.0));
	camera.setPosition(math::loadVector(7.0, 2.0, 0.0));
	camera.setUp(math::loadVector(0.0f, 1.0, 0.0));
	camera.setPlaneDistance(1.0f);
	camera.setPlaneHeight(1.0f);
	camera.setResolutionX(resolutionX);
	camera.setResolutionY(resolutionY);
	camera.setSampleCount(samplesPerPixel);
	camera.setSampler(&randomSampler);

	// Create the raytracer
	rayTracer.initialize(1000 * MB, 100 * MB, 12, 10000, true);
	rayTracer.setBackgroundColor(getColor(0, 0, 0));
	rayTracer.setDeterministicSeedMode(DETERMINISTIC_SEED_MODE);
	rayTracer.setPathRecordingOutputDirectory("../../workspace/diagnostics/");

	if (TRACE_SINGLE_PIXEL) {
		rayTracer.tracePixel(1025, 921, &scene, &camera);
	}
	else {
		rayTracer.traceAll(&scene, &camera);
	}

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

	sceneBuffer.applyGammaCurve((math::real)(1.0 / 2.2));
	manta::SaveImageData(sceneBuffer.getBuffer(), sceneBuffer.getWidth(), sceneBuffer.getHeight(), imageFname.c_str());

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
