#include <demos.h>

#include <iostream>

using namespace manta;

// 17:15

enum CAMERA_POSITION {
	FACE_ON,
	LEFT_GENERAL,
	RIGHT_GENERAL
};

void manta_demo::samsungA8Demo(int samplesPerPixel, int resolutionX, int resolutionY) {
	// Top-level parameters
	constexpr bool LENS_SIMULATION = false;
	constexpr bool USE_ACCELERATION_STRUCTURE = true;
	constexpr bool DETERMINISTIC_SEED_MODE = false;
	constexpr bool TRACE_SINGLE_PIXEL = false;
	constexpr CAMERA_POSITION CAMERA_POSITION = FACE_ON;

	RayTracer rayTracer;
	Scene scene;

	// Load all textures
	TextureNode speakerGrillTexture;
	speakerGrillTexture.loadFile(TEXTURE_PATH "samsung_a8/speaker_grill.jpg", (math::real)2.2);

	TextureNode backPlateTexture;
	backPlateTexture.loadFile(TEXTURE_PATH "samsung_a8/back_plate.png", (math::real)2.2);

	// Load all object files
	ObjFileLoader phoneObj;
	bool result = phoneObj.readObjFile(MODEL_PATH "samsung_a8_reverse.obj");

	if (!result) {
		std::cout << "Could not open geometry file(s)" << std::endl;
		phoneObj.destroy();
		return;
	}

	// Create all materials
	LambertianBSDF lambert;

	PhongDistribution phongPhoneCase;
	phongPhoneCase.setPower(1024);

	PhongDistribution phongGlassTemp;
	phongGlassTemp.setPower(10000);

	PhongDistribution phongBlackPlastic;
	phongPhoneCase.setPower(2048);

	PhongDistribution phongBayDoor;
	phongBayDoor.setPower(256);

	PhongDistribution phongBronze;
	phongBronze.setPower(256);

	PhongDistribution phongSteel;
	phongSteel.setPower(1024);

	PhongDistribution phongFloor;
	phongFloor.setPower(256);

	PhongDistribution mattePlasticPhong;
	mattePlasticPhong.setPower(64);

	PhongDistribution mirrorPhong;
	mirrorPhong.setPower(20000);

	BilayerBSDF phoneCaseBSDF;
	phoneCaseBSDF.setDiffuseMaterial(&lambert);
	phoneCaseBSDF.setCoatingDistribution(&phongPhoneCase);
	phoneCaseBSDF.setDiffuse(getColor(0x0, 0x0, 0x0));
	phoneCaseBSDF.setSpecularAtNormal(math::loadVector(0.0, 0.0, 0.0));

	BilayerBSDF bayDoorBSDF;
	bayDoorBSDF.setDiffuseMaterial(&lambert);
	bayDoorBSDF.setCoatingDistribution(&phongPhoneCase);
	bayDoorBSDF.setDiffuse(getColor(0x10, 0x10, 0x10));
	bayDoorBSDF.setSpecularAtNormal(math::loadVector(0.0, 0.0, 0.0));

	BilayerBSDF speakerGrillBSDF;
	speakerGrillBSDF.setDiffuseMaterial(&lambert);
	speakerGrillBSDF.setCoatingDistribution(&phongBayDoor);
	speakerGrillBSDF.setDiffuseNode(&speakerGrillTexture);
	speakerGrillBSDF.setDiffuse(math::loadVector(0.25, 0.25, 0.25));
	speakerGrillBSDF.setSpecularAtNormal(math::loadVector(0.0, 0.0, 0.0));

	BilayerBSDF blackPlasticBSDF;
	blackPlasticBSDF.setDiffuseMaterial(&lambert);
	blackPlasticBSDF.setCoatingDistribution(&phongBlackPlastic);
	blackPlasticBSDF.setDiffuse(getColor(0x0, 0x0, 0x0));
	blackPlasticBSDF.setSpecularAtNormal(math::loadVector(0.0, 0.0, 0.0));

	BilayerBSDF mattePlasticBSDF;
	mattePlasticBSDF.setDiffuseMaterial(&lambert);
	mattePlasticBSDF.setCoatingDistribution(&mattePlasticPhong);
	mattePlasticBSDF.setDiffuse(getColor(0xBC, 0xBC, 0xBC));
	mattePlasticBSDF.setSpecularAtNormal(math::loadVector(0.0, 0.0, 0.0));

	BilayerBSDF floorBSDF;
	floorBSDF.setDiffuseMaterial(&lambert);
	floorBSDF.setCoatingDistribution(&phongFloor);
	floorBSDF.setDiffuse(getColor(255, 255, 255));
	floorBSDF.setSpecularAtNormal(math::loadVector(0.0, 0.0, 0.0));

	BilayerBSDF tempGlassBSDF;
	tempGlassBSDF.setDiffuseMaterial(&lambert);
	tempGlassBSDF.setCoatingDistribution(&phongGlassTemp);
	tempGlassBSDF.setDiffuse(getColor(0, 0, 0));
	tempGlassBSDF.setSpecularAtNormal(math::loadVector(0.0, 0.0, 0.0));

	BilayerBSDF backPlateBSDF;
	backPlateBSDF.setDiffuseMaterial(&lambert);
	backPlateBSDF.setCoatingDistribution(&phongBlackPlastic);
	backPlateBSDF.setDiffuseNode(&backPlateTexture);
	backPlateBSDF.setSpecularAtNormal(math::loadVector(0.0, 0.0, 0.0));

	DielectricMediaInterface fresnel;
	fresnel.setIorIncident((math::real)1.0);
	fresnel.setIorTransmitted((math::real)1.1);
	MicrofacetTransmissionBSDF simpleGlassBSDF;
	simpleGlassBSDF.setDistribution(&phongGlassTemp);
	simpleGlassBSDF.setMediaInterface(&fresnel);

	DielectricMediaInterface lensFresnel;
	lensFresnel.setIorIncident((math::real)1.0);
	lensFresnel.setIorTransmitted((math::real)1.5);
	MicrofacetTransmissionBSDF lensGlassBSDF;
	lensGlassBSDF.setDistribution(&phongGlassTemp);
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

	SimpleBSDFMaterial *tempGlassMaterial = rayTracer.getMaterialManager()->newMaterial<SimpleBSDFMaterial>();
	tempGlassMaterial->setName("Glass");
	tempGlassMaterial->setReflectance(getColor(255, 255, 255));
	tempGlassMaterial->setBSDF(&simpleGlassBSDF);

	SimpleBSDFMaterial *tempLensGlassMaterial = rayTracer.getMaterialManager()->newMaterial<SimpleBSDFMaterial>();
	tempLensGlassMaterial->setName("LensGlass");
	tempLensGlassMaterial->setReflectance(getColor(255, 255, 255));
	tempLensGlassMaterial->setBSDF(&lensGlassBSDF);

	SimpleBSDFMaterial *speakerGrillMaterial = rayTracer.getMaterialManager()->newMaterial<SimpleBSDFMaterial>();
	speakerGrillMaterial->setName("SpeakerGrill");
	speakerGrillMaterial->setReflectance(getColor(255, 255, 255));
	speakerGrillMaterial->setBSDF(&speakerGrillBSDF);

	SimpleBSDFMaterial *screenMaskMaterial = rayTracer.getMaterialManager()->newMaterial<SimpleBSDFMaterial>();
	screenMaskMaterial->setName("ScreenMask");
	screenMaskMaterial->setReflectance(getColor(10, 10, 10));
	screenMaskMaterial->setBSDF(&lambert);

	SimpleBSDFMaterial *mattePlasticMaterial = rayTracer.getMaterialManager()->newMaterial<SimpleBSDFMaterial>();
	mattePlasticMaterial->setName("MattePlastic");
	mattePlasticMaterial->setReflectance(getColor(255, 255, 255));
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

	SimpleBSDFMaterial *strongLight = rayTracer.getMaterialManager()->newMaterial<SimpleBSDFMaterial>();
	strongLight->setEmission(math::loadVector(2.0, 2.0, 2.0));
	strongLight->setReflectance(math::constants::Zero);
	strongLight->setName("StrongLight");

	SimpleBSDFMaterial *weakLight = rayTracer.getMaterialManager()->newMaterial<SimpleBSDFMaterial>();
	weakLight->setEmission(math::loadVector(1.0, 1.0, 1.0));
	weakLight->setReflectance(math::constants::Zero);
	weakLight->setName("WeakLight");

	// Create all scene geometry
	Mesh phone;
	phone.loadObjFileData(&phoneObj, rayTracer.getMaterialManager(), defaultMaterial->getIndex(), 0);

	// Destroy file loaders
	phoneObj.destroy();

	KDTree kdtree;
	kdtree.initialize(150, math::constants::Zero);
	kdtree.analyze(&phone, 4);

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

	if (CAMERA_POSITION == FACE_ON) {
		cameraPos = math::loadVector(0, 70, 0);
		up = math::loadVector(0, 0, -1);
	}
	else if (CAMERA_POSITION == LEFT_GENERAL) {
		cameraPos = math::loadVector(-39.408, 12.216, 45.728);
	}
	else if (CAMERA_POSITION == RIGHT_GENERAL) {
		cameraPos = math::loadVector(+39.408, 12.216, 45.728);
	}

	math::Vector dir = math::normalize(math::sub(target, cameraPos));
	up = math::cross(math::cross(dir, up), dir);
	up = math::normalize(up);

	cameraPos = math::sub(cameraPos, math::mul(dir, math::loadScalar(1.0)));

	CameraRayEmitterGroup *group;
	manta::SimpleLens lens;
	lens.initialize();
	lens.setPosition(cameraPos);
	lens.setDirection(dir);
	lens.setUp(up);
	lens.setRadius(1.0);
	lens.setSensorResolutionX(resolutionX);
	lens.setSensorResolutionY(resolutionY);
	lens.setSensorHeight(22.0);
	lens.setSensorWidth(22.0 * (resolutionX / (math::real)resolutionY));
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
		math::real lensHeight = 1.0;
		math::real focusDistance = 11.0;

		Aperture *aperture = lens.getAperture();
		aperture->setRadius((math::real)0.007);
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
	rayTracer.initialize(800 * MB, 5 * MB, 12, 10000, true);
	rayTracer.setBackgroundColor(getColor(0, 0, 0));
	rayTracer.setDeterministicSeedMode(DETERMINISTIC_SEED_MODE);
	rayTracer.setPathRecordingOutputDirectory("../../workspace/diagnostics/");

	if (TRACE_SINGLE_PIXEL) {
		rayTracer.tracePixel(1706, 1303, &scene, group);
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
