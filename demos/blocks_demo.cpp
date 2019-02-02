#include <demos.h>

using namespace manta;

void manta_demo::blocksDemo(int samplesPerPixel, int resolutionX, int resolutionY) {
	Scene scene;

	RayTracer rayTracer;

	ObjFileLoader boxCityObj;
	bool result = boxCityObj.readObjFile(MODEL_PATH "blocks_floor.obj");

	TextureNode map;
	map.loadFile(TEXTURE_PATH "blocks.png", (math::real)2.2);

	TextureNode blockSpecular;
	blockSpecular.loadFile(TEXTURE_PATH "blocks_specular.png", (math::real)2.2);

	// Create all materials
	SingleColorNode whiteNode(getColor(255, 255, 255));
	SimpleLambertMaterial *wallMaterial = rayTracer.getMaterialManager()->newMaterial<SimpleLambertMaterial>();
	wallMaterial->setName("BlockSimple");
	wallMaterial->setEmission(math::constants::Zero);
	wallMaterial->setDiffuseNode(&whiteNode);

	PhongPhongBilayerMaterial *blockMaterialPhong = rayTracer.getMaterialManager()->newMaterial<PhongPhongBilayerMaterial>();
	DielectricMediaInterface *blockFresnel = blockMaterialPhong->getCoatingFresnel();
	blockFresnel->setIorIncident(1.0);
	blockFresnel->setIorTransmitted(1.6);
	blockMaterialPhong->setName("Block");
	blockMaterialPhong->setEmission(math::constants::Zero);
	blockMaterialPhong->setDiffuseColor(getColor(255, 255, 255));
	blockMaterialPhong->setSpecularColor(getColor(255, 255, 255));
	blockMaterialPhong->getDiffuseBSDF()->setPower((math::real)4.0);
	blockMaterialPhong->getSpecularBSDF()->setPower((math::real)512.0);
	blockMaterialPhong->setDiffuseNode(&map);
	blockMaterialPhong->getSpecularBSDF()->setPowerNode(&blockSpecular);

	PhongPhongBilayerMaterial *letterMaterialPhong = rayTracer.getMaterialManager()->newMaterial<PhongPhongBilayerMaterial>();
	DielectricMediaInterface *letterFresnel = letterMaterialPhong->getCoatingFresnel();
	letterFresnel->setIorIncident(1.0);
	letterFresnel->setIorTransmitted(1.6);
	letterMaterialPhong->setName("Letters");
	letterMaterialPhong->setEmission(math::constants::Zero);
	letterMaterialPhong->setDiffuseColor(getColor(255, 255, 255));
	letterMaterialPhong->setSpecularColor(getColor(255, 255, 255));
	letterMaterialPhong->getDiffuseBSDF()->setPower((math::real)4.0);
	letterMaterialPhong->getSpecularBSDF()->setPower((math::real)32.0);
	letterMaterialPhong->setDiffuseNode(&map);
	letterMaterialPhong->getSpecularBSDF()->setPowerNode(&blockSpecular);

	SingleColorNode greyNode(getColor(100, 100, 100));
	SimpleLambertMaterial *groundMaterial = rayTracer.getMaterialManager()->newMaterial<SimpleLambertMaterial>();
	groundMaterial->setName("GroundSimple");
	groundMaterial->setEmission(math::constants::Zero);
	groundMaterial->setDiffuseNode(&greyNode);

	PhongPhongBilayerMaterial *groundMaterialPhong = rayTracer.getMaterialManager()->newMaterial<PhongPhongBilayerMaterial>();
	DielectricMediaInterface *groundFresnel = groundMaterialPhong->getCoatingFresnel();
	groundFresnel->setIorIncident(1.0);
	groundFresnel->setIorTransmitted(1.6);
	groundMaterialPhong->setName("Ground");
	groundMaterialPhong->setEmission(math::constants::Zero);
	groundMaterialPhong->setDiffuseColor(getColor(255, 255, 255));
	groundMaterialPhong->setSpecularColor(getColor(255, 255, 255));
	groundMaterialPhong->getDiffuseBSDF()->setPower((math::real)8.0);
	groundMaterialPhong->getSpecularBSDF()->setPower((math::real)128.0);

	SimpleLambertMaterial outdoorLight;
	outdoorLight.setEmission(math::loadVector(9, 8, 8));
	outdoorLight.setDiffuseColor(math::constants::Zero);
	//outdoorLight.setSpecularColor(math::constants::Zero);

	SimpleLambertMaterial outdoorTopLightMaterial;
	outdoorTopLightMaterial.setEmission(math::loadVector(5, 5, 5));
	outdoorTopLightMaterial.setDiffuseColor(math::constants::Zero);
	//outdoorTopLightMaterial.setSpecularColor(math::constants::Zero);

	// Create all scene geometry
	Mesh boxCity;
	boxCity.loadObjFileData(&boxCityObj, rayTracer.getMaterialManager());
	boxCity.setFastIntersectEnabled(false);
	boxCity.setFastIntersectRadius((math::real)4.0);

	SpherePrimitive outdoorLightGeometry;
	outdoorLightGeometry.setRadius((math::real)10.0);
	outdoorLightGeometry.setPosition(math::loadVector(20, 30.0, -13.5));

	SpherePrimitive outdoorTopLightGeometry;
	outdoorTopLightGeometry.setRadius((math::real)10.0);
	//outdoorTopLightGeometry.setRadius((math::real)20.0);
	outdoorTopLightGeometry.setPosition(math::loadVector(10, 20.0, -13.5));

	SpherePrimitive groundGeometry;
	groundGeometry.setRadius((math::real)500000.01);
	groundGeometry.setPosition(math::loadVector(0.0, -500000, 0));

	SpherePrimitive groundLightGeometry;
	groundLightGeometry.setRadius((math::real)50000.0 - 1);
	groundLightGeometry.setPosition(math::loadVector(0.0, -50000, 0));

	// Create scene objects
	//SceneObject *smallHouseObject = scene.createSceneObject();
	//smallHouseObject->setGeometry(&smallHouse);
	//smallHouseObject->setMaterial(&wallMaterial);

	Octree octree;
	octree.initialize(50.0, math::loadVector(0, 0, 0));
	//octree.analyze(&boxCity, 50);
	//octree.writeToObjFile("../../workspace/test_results/blocks_octree.obj", nullptr);

	KDTree kdtree;
	kdtree.initialize((math::real)500.0, math::constants::Zero);
	kdtree.analyze(&boxCity, 4);

	constexpr bool useOctree = true;

	SceneObject *boxCityObject = scene.createSceneObject();
	if (useOctree) boxCityObject->setGeometry(&kdtree);
	else boxCityObject->setGeometry(&boxCity);
	boxCityObject->setDefaultMaterial(wallMaterial);

	//SceneObject *ground = scene.createSceneObject();
	//ground->setGeometry(&groundGeometry);
	//ground->setDefaultMaterial(&groundMaterial);

	//SceneObject *groundLight = scene.createSceneObject();
	//groundLight->setGeometry(&groundLightGeometry);
	//groundLight->setMaterial(&outdoorLight);

	SceneObject *outdoorTopLightObject = scene.createSceneObject();
	outdoorTopLightObject->setGeometry(&outdoorTopLightGeometry);
	outdoorTopLightObject->setDefaultMaterial(&outdoorTopLightMaterial);

	//SceneObject *lightSource = scene.createSceneObject();
	//lightSource->setGeometry(&outdoorLightGeometry);
	//lightSource->setMaterial(&outdoorLight);

	math::Vector cameraPos = math::loadVector(15.4473, 4.59977, 13.2961);
	math::Vector target = math::loadVector(2.63987, 3.55547, 2.42282);

	constexpr bool regularCamera = false;

	CameraRayEmitterGroup *group;

	// Create the camera

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

	if (regularCamera) {
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

	// Run the ray tracer
	rayTracer.initialize(1000 * MB, 50 * MB, 12, 10000, true);
	rayTracer.setBackgroundColor(getColor(255, 255, 255));
	//rayTracer.setBackgroundColor(getColor(0.0, 0.0, 0.0));
	//rayTracer.setDeterministicSeedMode(true);
	rayTracer.traceAll(&scene, group);
	//rayTracer.tracePixel(817, 689, &scene, &camera);

	// Output the results to a scene buffer
	SceneBuffer sceneBuffer;
	group->fillSceneBuffer(&sceneBuffer);

	// Clean up the camera
	group->destroyRays();
	group->destroyEmitters();
	delete group;

	std::string fname = createUniqueRenderFilename("blocks_demo", samplesPerPixel);
	std::string imageFname = std::string(RENDER_OUTPUT) + "bitmap/" + fname + ".bmp";
	std::string rawFname = std::string(RENDER_OUTPUT) + "raw/" + fname + ".fpm";

	RawFile rawFile;
	rawFile.writeRawFile(rawFname.c_str(), &sceneBuffer);
	editImage(&sceneBuffer, imageFname);

	sceneBuffer.destroy();
	rayTracer.destroy();

	kdtree.destroy();
	octree.destroy();
}
