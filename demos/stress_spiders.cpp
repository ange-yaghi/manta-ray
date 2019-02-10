#include <demos.h>

#include <iostream>

using namespace manta;

void manta_demo::stressSpidersDemo(int samplesPerPixel, int resolutionX, int resolutionY) {
	Scene scene;

	// Load all object files
	ObjFileLoader stressSpidersObj;
	bool result = stressSpidersObj.readObjFile(MODEL_PATH "stress_spiders.obj");

	RayTracer rayTracer;

	// Create all materials
	/*
	PhongPhongBilayerMaterial *spiderMaterial = rayTracer.getMaterialManager()->newMaterial<PhongPhongBilayerMaterial>();
	spiderMaterial->setEmission(math::mul(getColor(0xFF, 0x08, 0x14), math::loadScalar(0.0)));
	spiderMaterial->setDiffuseColor(getColor(0xf1, 0xc4, 0x0f));
	spiderMaterial->setDiffuseColor(getColor(0x1B, 0x23, 0x2E));
	spiderMaterial->setSpecularColor(getColor(70, 70, 70));
	spiderMaterial->setSpecularColor(getColor(0xFF, 0xFF, 0xFF));
	spiderMaterial->setSurfaceTransmission((math::real)0.5);
	spiderMaterial->getSpecularBSDF()->setPower((math::real)512);
	//spiderMaterial->setGloss((math::real)0.5);
	*/

	LambertianBSDF lambert;

	PhongDistribution spiderCoating;
	spiderCoating.setPower((math::real)8192);
	BilayerBSDF spiderBSDF;
	spiderBSDF.setCoatingDistribution(&spiderCoating);
	spiderBSDF.setDiffuseMaterial(&lambert);
	spiderBSDF.setDiffuse(getColor(0xf1, 0xc4, 0x0f));
	spiderBSDF.setSpecularAtNormal(math::loadVector(0.05, 0.05, 0.05));
	SimpleBSDFMaterial *spiderMaterial = rayTracer.getMaterialManager()->newMaterial<SimpleBSDFMaterial>();
	spiderMaterial->setName("StressSpiders");
	spiderMaterial->setBSDF(&spiderBSDF);

	PhongDistribution groundCoating;
	groundCoating.setPower((math::real)64);
	BilayerBSDF groundBSDF;
	groundBSDF.setCoatingDistribution(&groundCoating);
	groundBSDF.setDiffuseMaterial(&lambert);
	groundBSDF.setDiffuse(getColor(0xFF, 0xFF, 0xFF));
	groundBSDF.setSpecularAtNormal(math::loadVector(0.1, 0.1, 0.1));
	SimpleBSDFMaterial *groundMaterial = rayTracer.getMaterialManager()->newMaterial<SimpleBSDFMaterial>();
	groundMaterial->setName("Ground");
	groundMaterial->setBSDF(&groundBSDF);

	/*
	PhongPhongBilayerMaterial *groundMaterial = rayTracer.getMaterialManager()->newMaterial<PhongPhongBilayerMaterial>();
	groundMaterial->setEmission(math::constants::Zero);
	groundMaterial->setDiffuseColor(getColor(255, 255, 255));
	groundMaterial->setSpecularColor(math::constants::Zero);
	groundMaterial->setSurfaceTransmission((math::real)0.0);*/

	SimpleBSDFMaterial outdoorTopLightMaterial;
	outdoorTopLightMaterial.setEmission(math::loadVector(5, 5, 5));
	outdoorTopLightMaterial.setReflectance(math::constants::Zero);
	//outdoorTopLightMaterial.setSpecularColor(math::constants::Zero);

	// Create all scene geometry
	Mesh stressSpiders;
	stressSpiders.loadObjFileData(&stressSpidersObj, rayTracer.getMaterialManager(), spiderMaterial->getIndex());
	stressSpiders.setFastIntersectEnabled(false);

	stressSpidersObj.destroy();

	Octree stressSpidersOctree;
	stressSpidersOctree.initialize(32, math::constants::Zero);
	//stressSpidersOctree.analyze(&stressSpiders, 25);
	//stressSpidersOctree.writeToObjFile("../../workspace/test_results/stress_spiders_octree.obj", nullptr);

	KDTree kdtree;
	kdtree.initialize(100.0, math::constants::Zero);
	kdtree.analyze(&stressSpiders, 4);

	SpherePrimitive outdoorTopLightGeometry;
	outdoorTopLightGeometry.setRadius((math::real)10.0);
	//outdoorTopLightGeometry.setRadius((math::real)20.0);
	outdoorTopLightGeometry.setPosition(math::loadVector(19.45842, 12.42560, -13.78918));

	constexpr bool useOctree = true;

	// Create scene objects
	SceneObject *stressSpidersObject = scene.createSceneObject();
	if (useOctree) {
		stressSpidersObject->setGeometry(&kdtree);
	}
	else {
		stressSpidersObject->setGeometry(&stressSpiders);
	}
	stressSpidersObject->setDefaultMaterial(spiderMaterial);
	stressSpidersObject->setName("StressSpiders");

	SceneObject *outdoorTopLightObject = scene.createSceneObject();
	outdoorTopLightObject->setGeometry(&outdoorTopLightGeometry);
	outdoorTopLightObject->setDefaultMaterial(&outdoorTopLightMaterial);
	outdoorTopLightObject->setName("MainLamp");

	math::Vector cameraPos = math::loadVector(7.32725, 2.69770, 10.34150);
	math::Vector target = math::loadVector(0.0, 1.3, 0.0);

	math::Vector up = math::loadVector(0.0f, 1.0, 0.0);
	math::Vector dir = math::normalize(math::sub(target, cameraPos));
	up = math::cross(math::cross(dir, up), dir);
	up = math::normalize(up);

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
	lens.setSensorHeight(44.0);
	lens.setSensorWidth(44.0 * (resolutionX / (math::real)resolutionY));
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
		math::real focusDistance = 11.0;

		Aperture *aperture = lens.getAperture();
		aperture->setRadius((math::real)0.05);
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
	rayTracer.initialize(1000 * MB, 100 * MB, 12, 10000, true);
	rayTracer.setBackgroundColor(getColor(255, 255, 255));
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

	std::string fname = createUniqueRenderFilename("stress_spiders_demo", samplesPerPixel);
	std::string imageFname = std::string(RENDER_OUTPUT) + "bitmap/" + fname + ".bmp";
	std::string rawFname = std::string(RENDER_OUTPUT) + "raw/" + fname + ".fpm";

	RawFile rawFile;
	rawFile.writeRawFile(rawFname.c_str(), &sceneBuffer);
	//editImage(&sceneBuffer, imageFname);

	sceneBuffer.applyGammaCurve((math::real)(1.0 / 2.2));
	manta::SaveImageData(sceneBuffer.getBuffer(), sceneBuffer.getWidth(), sceneBuffer.getHeight(), imageFname.c_str());

	sceneBuffer.destroy();
	rayTracer.destroy();

	//stressSpidersOctree.destroy();
	stressSpiders.destroy();
	kdtree.destroy();

	std::cout << "Standard allocator memory leaks:     " << StandardAllocator::Global()->getLedger() << ", " << StandardAllocator::Global()->getCurrentUsage() << std::endl;
}
