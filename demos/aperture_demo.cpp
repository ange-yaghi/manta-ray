#include <demos.h>

using namespace manta;

void manta_demo::apertureDemo(int samplesPerPixel, int resolutionX, int resolutionY) {
	// Top-level parameters
	constexpr bool LENS_SIMULATION = true;
	constexpr bool USE_ACCELERATION_STRUCTURE = true;
	constexpr bool DETERMINISTIC_SEED_MODE = false;
	constexpr bool TRACE_SINGLE_PIXEL = false;

	Scene scene;
	RayTracer rayTracer;
	rayTracer.setMaterialManager(new MaterialManager);

	ObjFileLoader blocksObj;
	bool result = blocksObj.loadObjFile(MODEL_PATH "aperture_test.obj");

	if (!result) {
		std::cout << "Could not open geometry file" << std::endl;
		blocksObj.destroy();

		return;
	}

	SimpleBSDFMaterial *brightMaterial = rayTracer.getMaterialManager()->newMaterial<SimpleBSDFMaterial>();
	brightMaterial->setName("Bright");
	brightMaterial->setEmission(math::loadVector(100.f, 100.f, 100.f));
	brightMaterial->setReflectance(math::constants::Zero);

	SimpleBSDFMaterial *backdropMaterial = rayTracer.getMaterialManager()->newMaterial<SimpleBSDFMaterial>();
	backdropMaterial->setName("Backdrop");
	backdropMaterial->setReflectance(math::constants::Zero);

	// Create all scene geometry
	Mesh blocks;
	blocks.loadObjFileData(&blocksObj, rayTracer.getMaterialManager());
	blocks.setFastIntersectEnabled(false);
	blocksObj.destroy();

	SpherePrimitive outdoorTopLightGeometry;
	outdoorTopLightGeometry.setRadius((math::real)10.0);
	outdoorTopLightGeometry.setPosition(math::loadVector(10.f, 20.f, 5.5f));

	// Create scene objects
	KDTree kdtree;
	kdtree.initialize((math::real)500.0, math::constants::Zero);
	kdtree.analyzeWithProgress(&blocks, 4);

	SceneObject *boxCityObject = scene.createSceneObject();
	if (USE_ACCELERATION_STRUCTURE) boxCityObject->setGeometry(&kdtree);
	else boxCityObject->setGeometry(&blocks);
	boxCityObject->setDefaultMaterial(backdropMaterial);

	math::Vector cameraPos = math::loadVector(0.f, 13.f, 0.f);
	math::Vector target = math::loadVector(0.f, 0.f, 0.f);

	// Create the camera
	CameraRayEmitterGroup *group;

	math::Vector up = math::loadVector(0.0f, 0.0f, -1.0f);
	math::Vector dir = math::normalize(math::sub(target, cameraPos));
	up = math::cross(math::cross(dir, up), dir);
	up = math::normalize(up);

	manta::SimpleLens lens;
	manta::PolygonalAperture aperture;
	aperture.setRadius((math::real)0.25);
	aperture.initialize(3, 0.f, false);

	lens.setAperture(&aperture);
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
		aperture->setRadius((math::real)0.25); //0.18
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

	std::string fname = createUniqueRenderFilename("aperture_demo", samplesPerPixel);
	std::string imageFname = std::string(RENDER_OUTPUT) + "bitmap/" + fname + ".jpg";
	std::string rawFname = std::string(RENDER_OUTPUT) + "raw/" + fname + ".fpm";

	RawFile rawFile;
	rawFile.writeRawFile(rawFname.c_str(), &sceneBuffer);

	writeJpeg(imageFname.c_str(), &sceneBuffer, 95);

	blocks.destroy();
	sceneBuffer.destroy();
	rayTracer.destroy();
	aperture.destroy();

	kdtree.destroy();

	std::cout << "Standard allocator memory leaks:     " << StandardAllocator::Global()->getLedger() << ", " << StandardAllocator::Global()->getCurrentUsage() << std::endl;
}
