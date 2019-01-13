#include <demos.h>

#include <iostream>

using namespace manta;

void manta_demo::penDemo(int samplesPerPixel, int resolutionX, int resolutionY) {
	Scene scene;

	// Load all object files
	ObjFileLoader penChromeObj;
	bool result = penChromeObj.readObjFile(MODEL_PATH "pen_chrome_parts.obj");

	ObjFileLoader penPaintedObj;
	result = penPaintedObj.readObjFile(MODEL_PATH "pen_painted_parts.obj");

	//ObjFileLoader penGroundObj;
	//result = penGroundObj.readObjFile(MODEL_PATH "pen_ground.obj");

	RayTracer rayTracer;

	// Create all materials
	SimpleSpecularDiffuseMaterial *chrome = rayTracer.getMaterialManager()->newMaterial<SimpleSpecularDiffuseMaterial>();
	chrome->setEmission(math::mul(getColor(0xFF, 0x08, 0x14), math::loadScalar(0.0)));
	chrome->setDiffuseColor(getColor(10, 10, 10));
	chrome->setSpecularColor(getColor(200, 200, 200));
	chrome->setGloss((math::real)0.85);

	SimpleSpecularDiffuseMaterial *paint = rayTracer.getMaterialManager()->newMaterial<SimpleSpecularDiffuseMaterial>();
	paint->setEmission(math::mul(getColor(0xFF, 0x08, 0x14), math::loadScalar(0.0)));
	paint->setDiffuseColor(getColor(0xf1, 0xc4, 0x0f));
	paint->setDiffuseColor(getColor(0xFF, 0x08, 0x14));
	paint->setSpecularColor(getColor(70, 70, 70));
	paint->setGloss((math::real)0.8);

	//TextureMap woodTexture;
	//woodTexture.loadFile(TEXTURE_PATH "wood.png", (math::real)2.2);
	//SimpleSpecularDiffuseMaterial *wood = rayTracer.getMaterialManager()->newMaterial<SimpleSpecularDiffuseMaterial>();
	//wood->setEmission(math::mul(getColor(0xFF, 0x08, 0x14), math::loadScalar(0.0)));
	//wood->setDiffuseColor(getColor(0xFF, 0xFF, 0xFF));
	//wood->setSpecularColor(getColor(2, 2, 2));
	//wood->setGloss((math::real)0.3);
	//wood->setDiffuseMap(&woodTexture);

	SimpleSpecularDiffuseMaterial *groundMaterial = rayTracer.getMaterialManager()->newMaterial<SimpleSpecularDiffuseMaterial>();
	groundMaterial->setEmission(math::constants::Zero);
	groundMaterial->setDiffuseColor(getColor(255, 255, 255));
	groundMaterial->setSpecularColor(math::constants::Zero);

	SimpleSpecularDiffuseMaterial outdoorTopLightMaterial;
	outdoorTopLightMaterial.setEmission(math::loadVector(5, 5, 5));
	outdoorTopLightMaterial.setDiffuseColor(math::constants::Zero);
	outdoorTopLightMaterial.setSpecularColor(math::constants::Zero);

	// Create all scene geometry
	Mesh penChrome;
	penChrome.loadObjFileData(&penChromeObj, rayTracer.getMaterialManager(), chrome->getIndex(), 0);

	Mesh penPainted;
	penPainted.loadObjFileData(&penPaintedObj, rayTracer.getMaterialManager(), paint->getIndex(), penChrome.getFaceCount());

	// Destroy file loaders
	penChromeObj.destroy();
	penPaintedObj.destroy();

	//Mesh penGround;
	//penGround.loadObjFileData(&penGroundObj, wood->getIndex(), penChrome.getFaceCount() + penPainted.getFaceCount());

	penChrome.merge(&penPainted);
	//penChrome.merge(&penGround);

	Octree penOctree;
	penOctree.initialize(32, math::constants::Zero);
	penOctree.analyze(&penChrome, 25);
	//penOctree.writeToObjFile("../../workspace/test_results/pen_octree.obj", nullptr);

	SpherePrimitive light1Geometry;
	light1Geometry.setRadius((math::real)10.0);
	light1Geometry.setPosition(math::loadVector(19.45842, 12.42560, -13.78918));

	SpherePrimitive light2Geometry;
	light2Geometry.setRadius((math::real)10.0);
	light2Geometry.setPosition(math::loadVector(-20.255, 16.42560, -16.4256));

	SpherePrimitive light3Geometry;
	light3Geometry.setRadius((math::real)10.0);
	light3Geometry.setPosition(math::loadVector(3.28578, 12.42560, 17.577));

	SpherePrimitive groundGeometry;
	groundGeometry.setRadius((math::real)50000.0);
	groundGeometry.setPosition(math::loadVector(0.0, -50000.001, 0));

	constexpr bool useOctree = true;

	// Create scene objects
	SceneObject *penObject = scene.createSceneObject();
	if (useOctree) {
		penObject->setGeometry(&penOctree);
	}
	else {
		penObject->setGeometry(&penChrome);
	}
	penObject->setDefaultMaterial(paint);
	penObject->setName("Pen");

	SceneObject *ground = scene.createSceneObject();
	ground->setGeometry(&groundGeometry);
	ground->setDefaultMaterial(groundMaterial);
	ground->setName("Ground");

	SceneObject *light1Object = scene.createSceneObject();
	light1Object->setGeometry(&light1Geometry);
	light1Object->setDefaultMaterial(&outdoorTopLightMaterial);
	light1Object->setName("Light1");

	SceneObject *light2Object = scene.createSceneObject();
	light2Object->setGeometry(&light2Geometry);
	light2Object->setDefaultMaterial(&outdoorTopLightMaterial);
	light2Object->setName("Light2");

	SceneObject *light3Object = scene.createSceneObject();
	light3Object->setGeometry(&light3Geometry);
	light3Object->setDefaultMaterial(&outdoorTopLightMaterial);
	light3Object->setName("Light3");

	math::Vector cameraPos = math::loadVector(9.436, 1.0, 4.5370);
	math::Vector target = math::loadVector(1.64452, 0.35547, 0.0);

	math::Vector up = math::loadVector(0.0f, 1.0, 0.0);
	math::Vector dir = math::normalize(math::sub(target, cameraPos));
	up = math::cross(math::cross(dir, up), dir);
	up = math::normalize(up);

	cameraPos = math::sub(cameraPos, math::mul(dir, math::loadScalar(3.9)));

	// Create the camera
	constexpr bool regularCamera = false;
	CameraRayEmitterGroup *group;
	manta::SimpleLens lens;
	lens.initialize();
	lens.setPosition(cameraPos);
	lens.setDirection(dir);
	lens.setUp(up);
	lens.setRadius(1.0);
	lens.setSensorResolutionX(resolutionX);
	lens.setSensorResolutionY(resolutionY);
	lens.setSensorHeight(30.0);
	lens.setSensorWidth(30.0 * (resolutionX / (math::real)resolutionY));
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
		math::real focusDistance = 11.0;

		Aperture *aperture = lens.getAperture();
		aperture->setRadius((math::real)0.05); // 0.09
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
	rayTracer.initialize(800 * MB, 5 * MB, 12, 10000, true);
	rayTracer.setBackgroundColor(getColor(0, 0, 0));
	//rayTracer.setDeterministicSeedMode(true);
	//rayTracer.tracePixel(819, 199, &scene, &camera);
	//rayTracer.tracePixel(702, 236, &scene, &camera);
	//rayTracer.tracePixel(809, 211, &scene, &camera);
	//rayTracer.tracePixel(793, 224, &scene, &camera);
	//rayTracer.tracePixel(656, 185, &scene, &camera);
	//rayTracer.tracePixel(769, 318, &scene, &camera);
	//rayTracer.tracePixel(742, 218, &scene, &camera);
	//rayTracer.tracePixel(736, 331, &scene, &camera);

	// Leaks
	//rayTracer.tracePixel(1281, 900, &scene, &camera);
	//rayTracer.tracePixel(1456, 1230, &scene, &camera);
	//rayTracer.tracePixel(616, 1459, &scene, &camera);

	rayTracer.traceAll(&scene, group);

	// Output the results to a scene buffer
	SceneBuffer sceneBuffer;
	group->fillSceneBuffer(&sceneBuffer);

	// Clean up the camera
	group->destroyRays();
	group->destroyEmitters();

	std::string fname = createUniqueRenderFilename("pen_demo", samplesPerPixel);
	std::string imageFname = std::string(RENDER_OUTPUT) + "bitmap/" + fname + ".bmp";
	std::string rawFname = std::string(RENDER_OUTPUT) + "raw/" + fname + ".fpm";

	RawFile rawFile;
	rawFile.writeRawFile(rawFname.c_str(), &sceneBuffer);
	//editImage(&sceneBuffer, imageFname);

	sceneBuffer.applyGammaCurve((math::real)(1.0 / 2.2));
	manta::SaveImageData(sceneBuffer.getBuffer(), sceneBuffer.getWidth(), sceneBuffer.getHeight(), imageFname.c_str());

	sceneBuffer.destroy();
	rayTracer.destroy();

	penChrome.destroy();
	penPainted.destroy();

	penOctree.destroy();

	std::cout << "Standard allocator memory leaks:     " << StandardAllocator::Global()->getLedger() << ", " << StandardAllocator::Global()->getCurrentUsage() << std::endl;
}
