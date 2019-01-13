#include <demos.h>

using namespace manta;

void manta_demo::blocksDemo(int samplesPerPixel, int resolutionX, int resolutionY) {
	Scene scene;

	ObjFileLoader boxCityObj;
	bool result = boxCityObj.readObjFile(MODEL_PATH "blocks_floor.obj");

	TextureMap map;
	map.loadFile(TEXTURE_PATH "blocks.png", (math::real)2.2);

	// Create all materials
	SimpleSpecularDiffuseMaterial wallMaterial;
	wallMaterial.setEmission(math::constants::Zero);
	wallMaterial.setDiffuseColor(getColor(0xf1, 0xc4, 0x0f));
	wallMaterial.setDiffuseColor(getColor(255, 255, 255));
	wallMaterial.setSpecularColor(getColor(0, 0, 0));
	//wallMaterial.setSpecularColor(getColor(40, 40, 40));
	wallMaterial.setDiffuseMap(&map);

	SimpleSpecularDiffuseMaterial outdoorLight;
	outdoorLight.setEmission(math::loadVector(9, 8, 8));
	outdoorLight.setDiffuseColor(math::constants::Zero);
	outdoorLight.setSpecularColor(math::constants::Zero);

	SimpleSpecularDiffuseMaterial outdoorTopLightMaterial;
	outdoorTopLightMaterial.setEmission(math::loadVector(5, 5, 5));
	outdoorTopLightMaterial.setDiffuseColor(math::constants::Zero);
	outdoorTopLightMaterial.setSpecularColor(math::constants::Zero);

	SimpleSpecularDiffuseMaterial teapotMaterial;
	teapotMaterial.setEmission(math::constants::Zero);
	teapotMaterial.setDiffuseColor(getColor(1, 1, 1));
	teapotMaterial.setSpecularColor(getColor(100, 100, 100));

	SimpleSpecularDiffuseMaterial groundMaterial;
	groundMaterial.setEmission(math::constants::Zero);
	groundMaterial.setDiffuseColor(getColor(255, 255, 255));
	groundMaterial.setSpecularColor(math::constants::Zero);

	// Create all scene geometry
	Mesh boxCity;
	boxCity.loadObjFileData(&boxCityObj);
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
	octree.analyze(&boxCity, 50);
	octree.writeToObjFile("../../workspace/test_results/blocks_octree.obj", nullptr);

	constexpr bool useOctree = true;

	SceneObject *boxCityObject = scene.createSceneObject();
	if (useOctree) boxCityObject->setGeometry(&octree);
	else boxCityObject->setGeometry(&boxCity);
	boxCityObject->setDefaultMaterial(&wallMaterial);

	SceneObject *ground = scene.createSceneObject();
	ground->setGeometry(&groundGeometry);
	ground->setDefaultMaterial(&groundMaterial);

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

	if (regularCamera) {
		SSCameraRayEmitterGroup *camera = new SSCameraRayEmitterGroup;
		camera->setSamplingWidth(3);
		camera->setDirection(dir);
		camera->setPosition(cameraPos);
		camera->setUp(up);
		camera->setPlaneDistance(1.0f);
		camera->setPlaneHeight(1.0f);
		camera->setResolutionX(resolutionX);
		camera->setResolutionY(resolutionY);
		camera->setSamplesPerPixel(samplesPerPixel);

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
		camera->setSamplesPerPixel(1);
		camera->setExplicitSampleCount(samplesPerPixel);

		group = camera;
	}

	// Create the raytracer
	RayTracer rayTracer;
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
}
