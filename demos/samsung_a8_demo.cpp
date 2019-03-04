#include <demos.h>

#include <iostream>

using namespace manta;

void manta_demo::samsungA8Demo(int samplesPerPixel, int resolutionX, int resolutionY) {
	enum SCENE {
		FACE_ON_SCENE,
		UPRIGHT_SCENE,
		BANNER_SCENE
	};

	// Top-level parameters
	constexpr bool USE_ACCELERATION_STRUCTURE = true;
	constexpr bool DETERMINISTIC_SEED_MODE = false;
	constexpr bool TRACE_SINGLE_PIXEL = false;
	constexpr SCENE SCENE = BANNER_SCENE;
	constexpr bool ENABLE_SMUDGE = true;

	RayTracer rayTracer;
	Scene scene;

	// Load all textures
	TextureNode speakerGrillTexture;
	speakerGrillTexture.loadFile(TEXTURE_PATH "samsung_a8/speaker_grill.jpg", (math::real)2.2);

	TextureNode backPlateTexture;
	backPlateTexture.loadFile(TEXTURE_PATH "samsung_a8/back_plate.png", (math::real)2.2);

	TextureNode phoneScreenTexture;
	phoneScreenTexture.loadFile(TEXTURE_PATH "samsung_a8/phone_screen.png", (math::real)2.2);

	TextureNode groundRoughness;
	groundRoughness.loadFile(TEXTURE_PATH "chrome_roughness.jpg", (math::real)1.0);

	TextureNode smudgeMap;
	smudgeMap.loadFile(TEXTURE_PATH "samsung_a8/fingerprints_roughness_map.png", (math::real)1.0);

	// Load all object files
	ObjFileLoader phoneObj;
	bool result;

	if (SCENE == FACE_ON_SCENE) {
		result = phoneObj.readObjFile(MODEL_PATH "samsung_a8_face_on_scene.obj");
	}
	else if (SCENE == UPRIGHT_SCENE) {
		result = phoneObj.readObjFile(MODEL_PATH "samsung_a8_upright_scene.obj");
	}
	else if (SCENE == BANNER_SCENE) {
		result = phoneObj.readObjFile(MODEL_PATH "samsung_a8_banner_scene.obj");
	}

	if (!result) {
		std::cout << "Could not open geometry file(s)" << std::endl;
		phoneObj.destroy();
		return;
	}

	// Create all materials
	LambertianBSDF lambert;

	PhongDistribution phongPhoneCase;
	phongPhoneCase.setPower(1024);
	phongPhoneCase.setMinMapPower(8);
	if (ENABLE_SMUDGE && (SCENE == UPRIGHT_SCENE || SCENE == BANNER_SCENE)) phongPhoneCase.setPowerNode(&smudgeMap);

	PhongDistribution phongGlass;
	phongGlass.setPower(5000);
	phongGlass.setMinMapPower(200);
	if (ENABLE_SMUDGE && (SCENE == UPRIGHT_SCENE || SCENE == BANNER_SCENE)) phongGlass.setPowerNode(&smudgeMap);

	PhongDistribution phongBlackPlastic;
	phongBlackPlastic.setPower(5000);

	PhongDistribution phongBayDoor;
	phongBayDoor.setPower(256);

	PhongDistribution phongBronze;
	phongBronze.setPower(256);

	PhongDistribution phongSteel;
	phongSteel.setPower(1024);

	PhongDistribution phongFloor;
	phongFloor.setPower(256);
	phongFloor.setMinMapPower(240);
	if (ENABLE_SMUDGE && (SCENE == UPRIGHT_SCENE || SCENE == BANNER_SCENE)) phongFloor.setPowerNode(&groundRoughness);

	PhongDistribution mattePlasticPhong;
	mattePlasticPhong.setPower(64);

	PhongDistribution mirrorPhong;
	mirrorPhong.setPower(20000);

	BilayerBSDF phoneCaseBSDF;
	phoneCaseBSDF.setDiffuseMaterial(&lambert);
	phoneCaseBSDF.setCoatingDistribution(&phongPhoneCase);
	phoneCaseBSDF.setDiffuse(getColor(0x0, 0x0, 0x0));
	phoneCaseBSDF.setSpecularAtNormal(math::loadVector(0.01, 0.01, 0.01));

	BilayerBSDF bayDoorBSDF;
	bayDoorBSDF.setDiffuseMaterial(&lambert);
	bayDoorBSDF.setCoatingDistribution(&phongBayDoor);
	bayDoorBSDF.setDiffuse(getColor(0x0, 0x0, 0x0));
	bayDoorBSDF.setSpecularAtNormal(math::loadVector(0.0, 0.0, 0.0));

	BilayerBSDF speakerGrillBSDF;
	speakerGrillBSDF.setDiffuseMaterial(&lambert);
	speakerGrillBSDF.setCoatingDistribution(&phongBayDoor);
	speakerGrillBSDF.setDiffuseNode(&speakerGrillTexture);
	speakerGrillBSDF.setDiffuse(math::loadVector(0.1, 0.1, 0.1));
	speakerGrillBSDF.setSpecularAtNormal(math::loadVector(0.0, 0.0, 0.0));

	BilayerBSDF blackPlasticBSDF;
	blackPlasticBSDF.setDiffuseMaterial(&lambert);
	blackPlasticBSDF.setCoatingDistribution(&phongBlackPlastic);
	blackPlasticBSDF.setDiffuse(getColor(0x0, 0x0, 0x0));
	blackPlasticBSDF.setSpecularAtNormal(math::loadVector(0.05, 0.05, 0.05));

	MicrofacetReflectionBSDF mattePlasticBSDF;
	mattePlasticBSDF.setDistribution(&mattePlasticPhong);

	BilayerBSDF floorBSDF;
	floorBSDF.setDiffuseMaterial(&lambert);
	floorBSDF.setCoatingDistribution(&phongFloor);
	if (SCENE == UPRIGHT_SCENE || SCENE == BANNER_SCENE) {
		floorBSDF.setDiffuse(getColor(0, 0, 0));
		floorBSDF.setSpecularAtNormal(math::loadVector(0.05, 0.05, 0.05));
	}
	else if (SCENE == FACE_ON_SCENE) {
		floorBSDF.setDiffuse(getColor(255, 255, 255));
		floorBSDF.setSpecularAtNormal(math::loadVector(0.0, 0.0, 0.0));
	}

	BilayerBSDF backPlateBSDF;
	backPlateBSDF.setDiffuseMaterial(&lambert);
	backPlateBSDF.setCoatingDistribution(&phongBlackPlastic);
	backPlateBSDF.setDiffuseNode(&backPlateTexture);
	backPlateBSDF.setSpecularAtNormal(math::loadVector(0.0, 0.0, 0.0));

	DielectricMediaInterface fresnel;
	fresnel.setIorIncident((math::real)1.0);
	fresnel.setIorTransmitted((math::real)1.4);
	MicrofacetGlassBSDF simpleGlassBSDF;
	simpleGlassBSDF.setDistribution(&phongGlass);
	simpleGlassBSDF.setMediaInterface(&fresnel);

	DielectricMediaInterface lensFresnel;
	lensFresnel.setIorIncident((math::real)1.0);
	lensFresnel.setIorTransmitted((math::real)1.5);
	MicrofacetGlassBSDF lensGlassBSDF;
	lensGlassBSDF.setDistribution(&phongGlass);
	lensGlassBSDF.setMediaInterface(&lensFresnel);

	MicrofacetReflectionBSDF bronzeBSDF;
	bronzeBSDF.setDistribution(&phongBronze);

	MicrofacetReflectionBSDF steelBSDF;
	steelBSDF.setDistribution(&phongSteel);

	MicrofacetReflectionBSDF imageSensorBSDF;
	imageSensorBSDF.setDistribution(&mattePlasticPhong);

	MicrofacetReflectionBSDF mirrorBSDF;
	mirrorBSDF.setDistribution(&mirrorPhong);

	SimpleBSDFMaterial *defaultMaterial = rayTracer.getMaterialManager()->newMaterial<SimpleBSDFMaterial>();
	defaultMaterial->setName("Default");
	defaultMaterial->setBSDF(&lambert);

	SimpleBSDFMaterial *phoneCaseMaterial = rayTracer.getMaterialManager()->newMaterial<SimpleBSDFMaterial>();
	phoneCaseMaterial->setName("PhoneCase");
	phoneCaseMaterial->setReflectance(getColor(255, 255, 255));
	phoneCaseMaterial->setReflectanceNode(&smudgeMap);
	phoneCaseMaterial->setBSDF(&phoneCaseBSDF);

	SimpleBSDFMaterial *bayDoorMaterial = rayTracer.getMaterialManager()->newMaterial<SimpleBSDFMaterial>();
	bayDoorMaterial->setName("BayDoorMaterial");
	bayDoorMaterial->setReflectance(getColor(255, 255, 255));
	bayDoorMaterial->setBSDF(&bayDoorBSDF);

	SimpleBSDFMaterial *bronzeMaterial = rayTracer.getMaterialManager()->newMaterial<SimpleBSDFMaterial>();
	bronzeMaterial->setName("Bronze");
	bronzeMaterial->setReflectance(getColor(0xC0, 0xA5, 0x70));
	bronzeMaterial->setBSDF(&bronzeBSDF);

	SimpleBSDFMaterial *steelMaterial = rayTracer.getMaterialManager()->newMaterial<SimpleBSDFMaterial>();
	steelMaterial->setName("Steel");
	steelMaterial->setReflectance(getColor(255, 255, 255));
	steelMaterial->setBSDF(&steelBSDF);

	SimpleBSDFMaterial *blackPlasticMaterial = rayTracer.getMaterialManager()->newMaterial<SimpleBSDFMaterial>();
	blackPlasticMaterial->setName("BlackPlastic");
	blackPlasticMaterial->setReflectance(getColor(255, 255, 255));
	blackPlasticMaterial->setBSDF(&blackPlasticBSDF);

	SimpleBSDFMaterial *blackMaterial = rayTracer.getMaterialManager()->newMaterial<SimpleBSDFMaterial>();
	blackMaterial->setName("Black");
	blackMaterial->setReflectance(getColor(0, 0, 0));
	blackMaterial->setBSDF(nullptr);

	SimpleBSDFMaterial *floorMaterial = rayTracer.getMaterialManager()->newMaterial<SimpleBSDFMaterial>();
	floorMaterial->setName("Floor");
	floorMaterial->setReflectance(getColor(255, 255, 255));
	floorMaterial->setBSDF(&floorBSDF);

	SimpleBSDFMaterial *glassMaterial = rayTracer.getMaterialManager()->newMaterial<SimpleBSDFMaterial>();
	glassMaterial->setName("Glass");
	glassMaterial->setReflectance(getColor(255, 255, 255));
	glassMaterial->setBSDF(&simpleGlassBSDF);

	SimpleBSDFMaterial *lensGlassMaterial = rayTracer.getMaterialManager()->newMaterial<SimpleBSDFMaterial>();
	lensGlassMaterial->setName("LensGlass");
	lensGlassMaterial->setReflectance(getColor(255, 255, 255));
	lensGlassMaterial->setBSDF(&lensGlassBSDF);

	SimpleBSDFMaterial *speakerGrillMaterial = rayTracer.getMaterialManager()->newMaterial<SimpleBSDFMaterial>();
	speakerGrillMaterial->setName("SpeakerGrill");
	speakerGrillMaterial->setReflectance(getColor(255, 255, 255));
	speakerGrillMaterial->setBSDF(&speakerGrillBSDF);

	SimpleBSDFMaterial *screenMaskMaterial = rayTracer.getMaterialManager()->newMaterial<SimpleBSDFMaterial>();
	screenMaskMaterial->setName("ScreenMask");
	screenMaskMaterial->setReflectance(getColor(5, 5, 5));
	screenMaskMaterial->setBSDF(&lambert);

	SimpleBSDFMaterial *mattePlasticMaterial = rayTracer.getMaterialManager()->newMaterial<SimpleBSDFMaterial>();
	mattePlasticMaterial->setName("MattePlastic");
	mattePlasticMaterial->setReflectance(getColor(0x52, 0x4f, 0x51));
	mattePlasticMaterial->setBSDF(&mattePlasticBSDF);

	SimpleBSDFMaterial *imageSensorMaterial = rayTracer.getMaterialManager()->newMaterial<SimpleBSDFMaterial>();
	imageSensorMaterial->setName("ImageSensor");
	imageSensorMaterial->setReflectance(getColor(0x34, 0x2e, 0x38));
	imageSensorMaterial->setBSDF(&imageSensorBSDF);

	SimpleBSDFMaterial *mirrorMaterial = rayTracer.getMaterialManager()->newMaterial<SimpleBSDFMaterial>();
	mirrorMaterial->setName("Mirror");
	mirrorMaterial->setReflectance(getColor(255, 255, 255));
	mirrorMaterial->setBSDF(&mirrorBSDF);

	SimpleBSDFMaterial *flashColorMaterial = rayTracer.getMaterialManager()->newMaterial<SimpleBSDFMaterial>();
	flashColorMaterial->setName("FlashColor");
	flashColorMaterial->setReflectance(getColor(0xE7, 0xE0, 0x7F));
	flashColorMaterial->setBSDF(&lambert);

	SimpleBSDFMaterial *backPlateMaterial = rayTracer.getMaterialManager()->newMaterial<SimpleBSDFMaterial>();
	backPlateMaterial->setName("BackPlate");
	backPlateMaterial->setReflectance(getColor(255, 255, 255));
	backPlateMaterial->setBSDF(&backPlateBSDF);

	SimpleBSDFMaterial *screenMaterial = rayTracer.getMaterialManager()->newMaterial<SimpleBSDFMaterial>();
	screenMaterial->setName("Screen");
	screenMaterial->setEmissionNode(&phoneScreenTexture);
	screenMaterial->setEmission(math::loadVector(1.6, 1.6, 1.6));
	screenMaterial->setReflectance(getColor(3, 3, 3));
	screenMaterial->setBSDF(&lambert);

	SimpleBSDFMaterial *strongLight = rayTracer.getMaterialManager()->newMaterial<SimpleBSDFMaterial>();
	if (SCENE == UPRIGHT_SCENE || SCENE == BANNER_SCENE) strongLight->setEmission(math::loadVector(8.0, 8.0, 8.0));
	else if (SCENE == FACE_ON_SCENE) strongLight->setEmission(math::loadVector(5.0, 5.0, 5.0));
	strongLight->setReflectance(math::constants::Zero);
	strongLight->setName("StrongLight");

	SimpleBSDFMaterial *weakLight = rayTracer.getMaterialManager()->newMaterial<SimpleBSDFMaterial>();
	weakLight->setEmission(math::loadVector(1.9, 1.9, 1.9));
	weakLight->setReflectance(math::constants::Zero);
	weakLight->setName("WeakLight");

	SimpleBSDFMaterial *fillLight = rayTracer.getMaterialManager()->newMaterial<SimpleBSDFMaterial>();
	fillLight->setEmission(math::loadVector(11.4, 11.4, 11.4));
	fillLight->setReflectance(math::constants::Zero);
	fillLight->setName("FillLight");

	// Create all scene geometry
	Mesh phone;
	phone.loadObjFileData(&phoneObj, rayTracer.getMaterialManager(), defaultMaterial->getIndex(), 0);
	std::cout << phone.getTriangleFaceCount() << std::endl;
	//phone.findQuads();
	std::cout << phone.getQuadFaceCount() << ", " << phone.getTriangleFaceCount() << std::endl;

	// Destroy file loaders
	phoneObj.destroy();

	KDTree kdtree;
	kdtree.initialize(1000, math::constants::Zero);
	kdtree.analyze(&phone, 2);

	// Create scene objects
	SceneObject *phoneObject = scene.createSceneObject();
	if (USE_ACCELERATION_STRUCTURE) {
		phoneObject->setGeometry(&kdtree);
	}
	else {
		phoneObject->setGeometry(&phone);
	}
	phoneObject->setDefaultMaterial(defaultMaterial);
	phoneObject->setName("Phone");

	// Create the camera
	math::Vector cameraPos = math::loadVector(-39.408, 12.216, 45.728);
	math::Vector target = math::loadVector(0, 0, 0.0);
	math::Vector up = math::loadVector(0.0f, 1.0, 0.0);
	math::real cameraPlaneSize = 0.25;

	if (SCENE == FACE_ON_SCENE) {
		cameraPos = math::loadVector(0, 90, 0);
		up = math::loadVector(0, 0, -1);
	}
	else if (SCENE == UPRIGHT_SCENE) {
		target = math::loadVector(0, 8.8565, -8.9672);
		cameraPos = math::loadVector(0, 13.786, -28.987);
		cameraPlaneSize = 0.65;
	}
	else if (SCENE == BANNER_SCENE) {
		target = math::loadVector(7.66829, 7.63167, -18.7725);
		cameraPos = math::loadVector(8.2166, 8.00816, -31.4485);
		cameraPlaneSize = 0.4992;
	}

	math::Vector dir = math::normalize(math::sub(target, cameraPos));
	up = math::cross(math::cross(dir, up), dir);
	up = math::normalize(up);

	cameraPos = math::sub(cameraPos, math::mul(dir, math::loadScalar(1.0)));

	CameraRayEmitterGroup *group;
	RandomSampler sampler;
	StandardCameraRayEmitterGroup *camera = new StandardCameraRayEmitterGroup;
	camera->setSampler(&sampler);
	camera->setDirection(dir);
	camera->setPosition(cameraPos);
	camera->setUp(up);
	camera->setPlaneDistance(1.0f);
	camera->setPlaneHeight(cameraPlaneSize);
	if (SCENE == BANNER_SCENE) {
		camera->setResolutionX(1920);
		camera->setResolutionY(1080);
	}
	else {
		camera->setResolutionX(resolutionX);
		camera->setResolutionY(resolutionY);
	}
	camera->setSampleCount(samplesPerPixel);
	group = camera;

	// Create the raytracer
	rayTracer.initialize(1000 * MB, 50 * MB, 12, 10000, true);
	rayTracer.setBackgroundColor(getColor(0, 0, 0));
	rayTracer.setDeterministicSeedMode(DETERMINISTIC_SEED_MODE);
	rayTracer.setPathRecordingOutputDirectory("../../workspace/diagnostics/");

	if (TRACE_SINGLE_PIXEL) {
		rayTracer.tracePixel(1044, 1063, &scene, group);
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

	std::string fname = createUniqueRenderFilename("samsung_a8_demo", samplesPerPixel);
	std::string imageFname = std::string(RENDER_OUTPUT) + "bitmap/" + fname + ".bmp";
	std::string rawFname = std::string(RENDER_OUTPUT) + "raw/" + fname + ".fpm";

	RawFile rawFile;
	rawFile.writeRawFile(rawFname.c_str(), &sceneBuffer);

	sceneBuffer.applyGammaCurve((math::real)(1.0 / 2.2));
	manta::SaveImageData(sceneBuffer.getBuffer(), sceneBuffer.getWidth(), sceneBuffer.getHeight(), imageFname.c_str());

	sceneBuffer.destroy();
	rayTracer.destroy();
	phone.destroy();
	kdtree.destroy();

	std::cout << "Standard allocator memory leaks:     " << StandardAllocator::Global()->getLedger() << ", " << StandardAllocator::Global()->getCurrentUsage() << std::endl;
}
