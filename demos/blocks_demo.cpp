#include <demos.h>

using namespace manta;

void manta_demo::blocksDemo(int samplesPerPixel, int resolutionX, int resolutionY) {
	// Top-level parameters
	constexpr bool LENS_SIMULATION = true;
	constexpr bool USE_ACCELERATION_STRUCTURE = true;
	constexpr bool DETERMINISTIC_SEED_MODE = false;
	constexpr bool TRACE_SINGLE_PIXEL = false;

	Scene scene;
	RayTracer rayTracer;

	ObjFileLoader blocksObj;
	bool result = blocksObj.readObjFile(MODEL_PATH "blocks_floor.obj");

	if (!result) {
		std::cout << "Could not open geometry file" << std::endl;
		blocksObj.destroy();

		return;
	}

	TextureNode map;
	map.loadFile(TEXTURE_PATH "blocks.png", (math::real)2.2);

	TextureNode blockSpecular;
	blockSpecular.loadFile(TEXTURE_PATH "blocks_specular.png", (math::real)2.2);

	PhongDistribution phongDist;
	phongDist.setPower(4096);

	PhongDistribution phongDist2;
	phongDist2.setPower(16);

	MicrofacetReflectionBSDF bsdf2;
	bsdf2.setDistribution(&phongDist);

	LambertianBSDF lambert;

	DielectricMediaInterface fresnel;
	fresnel.setIorIncident((math::real)1.0);
	fresnel.setIorTransmitted((math::real)1.5);

	BilayerBSDF blockBSDF;
	blockBSDF.setDiffuseMaterial(&lambert);
	blockBSDF.setCoatingDistribution(&phongDist);
	blockBSDF.setDiffuseNode(&map);
	blockBSDF.setDiffuse(getColor(0xFF, 0xFF, 0xFF));
	blockBSDF.setSpecularAtNormal(math::loadVector(0.1f, 0.1f, 0.1f));

	BilayerBSDF floorBSDF;
	floorBSDF.setDiffuseMaterial(&lambert);
	floorBSDF.setCoatingDistribution(&phongDist2);
	floorBSDF.setDiffuse(getColor(0xFF, 0xFF, 0xFF));
	floorBSDF.setSpecularAtNormal(math::loadVector(0.75f, 0.75f, 0.75f));

	// Create all materials
	SingleColorNode whiteNode(getColor(255, 255, 255));
	SimpleBSDFMaterial *simpleBlockMaterial = rayTracer.getMaterialManager()->newMaterial<SimpleBSDFMaterial>();
	simpleBlockMaterial->setName("Block");
	simpleBlockMaterial->setEmission(math::constants::Zero);
	simpleBlockMaterial->setReflectanceNode(&whiteNode);
	simpleBlockMaterial->setBSDF(&blockBSDF);

	SimpleBSDFMaterial *simpleLetterMaterial = rayTracer.getMaterialManager()->newMaterial<SimpleBSDFMaterial>();
	simpleLetterMaterial->setName("Letters");
	simpleLetterMaterial->setEmission(math::constants::Zero);
	simpleLetterMaterial->setReflectanceNode(&whiteNode);
	simpleLetterMaterial->setBSDF(&blockBSDF);

	SimpleBSDFMaterial *simpleGroundMaterial = rayTracer.getMaterialManager()->newMaterial<SimpleBSDFMaterial>();
	simpleGroundMaterial->setName("Ground");
	simpleGroundMaterial->setEmission(math::constants::Zero);
	simpleGroundMaterial->setReflectanceNode(&whiteNode);
	simpleGroundMaterial->setBSDF(&floorBSDF);

	SimpleBSDFMaterial outdoorTopLightMaterial;
	outdoorTopLightMaterial.setEmission(math::loadVector(5.f, 5.f, 5.f));
	outdoorTopLightMaterial.setReflectance(math::constants::Zero);

	// Create all scene geometry
	Mesh blocks;
	blocks.loadObjFileData(&blocksObj, rayTracer.getMaterialManager());
	blocks.setFastIntersectEnabled(false);

	SpherePrimitive outdoorTopLightGeometry;
	outdoorTopLightGeometry.setRadius((math::real)10.0);
	outdoorTopLightGeometry.setPosition(math::loadVector(10.f, 20.f, 5.5f));

	// Create scene objects
	KDTree kdtree;
	kdtree.initialize((math::real)500.0, math::constants::Zero);
	kdtree.analyze(&blocks, 4);

	SceneObject *boxCityObject = scene.createSceneObject();
	if (USE_ACCELERATION_STRUCTURE) boxCityObject->setGeometry(&kdtree);
	else boxCityObject->setGeometry(&blocks);
	boxCityObject->setDefaultMaterial(simpleBlockMaterial);

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
	lens.setRadius(1.0);
	lens.setSensorResolutionX(resolutionX);
	lens.setSensorResolutionY(resolutionY);
	lens.setSensorHeight(10.0f);
	lens.setSensorWidth(10.0f * (resolutionX / (math::real)resolutionY));
	lens.update();

	RandomSampler sampler;

	if (!LENS_SIMULATION) {
		StandardCameraRayEmitterGroup *camera = new StandardCameraRayEmitterGroup;
		camera->setSampler(&sampler);
		camera->setDirection(dir);
		camera->setPosition(cameraPos);
		camera->setUp(up);
		camera->setPlaneDistance(1.0f);
		camera->setPlaneHeight(1.0f);
		camera->setResolutionX(resolutionX);
		camera->setResolutionY(resolutionY);
		camera->setSampleCount(samplesPerPixel);

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

	// Output the results to a scene buffer
	ImagePlane sceneBuffer;

	// Run the ray tracer
	rayTracer.initialize(200 * MB, 50 * MB, 12, 100, true);
	rayTracer.setBackgroundColor(getColor(0, 0, 0));
	rayTracer.setDeterministicSeedMode(DETERMINISTIC_SEED_MODE);

	if (TRACE_SINGLE_PIXEL) {
		rayTracer.tracePixel(1286, 1157, &scene, group, &sceneBuffer);
	}
	else {
		rayTracer.traceAll(&scene, group, &sceneBuffer);
	}

	// Clean up the camera
	delete group;

	std::string fname = createUniqueRenderFilename("blocks_demo", samplesPerPixel);
	std::string imageFname = std::string(RENDER_OUTPUT) + "bitmap/" + fname + ".jpg";
	std::string rawFname = std::string(RENDER_OUTPUT) + "raw/" + fname + ".fpm";

	RawFile rawFile;
	rawFile.writeRawFile(rawFname.c_str(), &sceneBuffer);

	sceneBuffer.applyGammaCurve((math::real)(1.0 / 2.2));
	writeJpeg(imageFname.c_str(), &sceneBuffer, 95);

	sceneBuffer.destroy();
	rayTracer.destroy();

	kdtree.destroy();
}
