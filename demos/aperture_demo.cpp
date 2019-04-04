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

	ObjFileLoader blocksObj;
	bool result = blocksObj.readObjFile(MODEL_PATH "aperture_test.obj");

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
	//manta::CircularAperture aperture;
	manta::PolygonalAperture aperture;
	aperture.setRadius((math::real)0.25);
	aperture.initialize(8, 0.f, false);

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
	ImagePlane planeWaveApproximation;

	// Run the ray tracer
	rayTracer.initialize(200 * MB, 50 * MB, 12, 100, true);
	rayTracer.setBackgroundColor(getColor(0, 0, 0));
	rayTracer.setDeterministicSeedMode(DETERMINISTIC_SEED_MODE);

	if (TRACE_SINGLE_PIXEL) {
		rayTracer.tracePixel(1286, 1157, &scene, group, &sceneBuffer);
	}
	else {
		std::cout << "Pass 1 ================================" << std::endl;
		rayTracer.traceAll(&scene, group, &sceneBuffer);

		if (LENS_SIMULATION) {
			std::cout << "Pass 2 ================================" << std::endl;
			group->setSampleCount(samplesPerPixel / 2);
			lens.getAperture()->setRadius(0.01f);
			rayTracer.traceAll(&scene, group, &planeWaveApproximation);
		}
		else {
			sceneBuffer.clone(&planeWaveApproximation);
		}
	}

	// Clean up the camera
	delete group;

	// Try convolution
	ComplexMap2D imageMap, imageMapSafe;
	imageMap.copy(&planeWaveApproximation, 0);

	Margins margins;
	imageMap.resizeSafe(&imageMapSafe, &margins);
	imageMap.destroy();

	lens.getAperture()->setRadius(5.5f);

	FraunhoferDiffraction testFraun;
	testFraun.generate(&aperture, imageMapSafe.getWidth(), 0.15f);

	ComplexMap2D diffractionR, diffractionG, diffractionB;
	diffractionR.initialize(imageMapSafe.getWidth(), imageMapSafe.getHeight());
	diffractionG.initialize(imageMapSafe.getWidth(), imageMapSafe.getHeight());
	diffractionB.initialize(imageMapSafe.getWidth(), imageMapSafe.getHeight());
	for (int i = 0; i < imageMapSafe.getWidth(); i++) {
		for (int j = 0; j < imageMapSafe.getHeight(); j++) {
			diffractionR.set(math::Complex(math::getX(testFraun.getDiffractionPattern()->get(i, j)), 0.0f), i, j);
			diffractionG.set(math::Complex(math::getY(testFraun.getDiffractionPattern()->get(i, j)), 0.0f), i, j);
			diffractionB.set(math::Complex(math::getZ(testFraun.getDiffractionPattern()->get(i, j)), 0.0f), i, j);
		}
	}

	ComplexMap2D baseFt, filterFtR, filterFtG, filterFtB;
	imageMapSafe.fft(&baseFt); imageMapSafe.destroy();
	diffractionR.fft(&filterFtR); diffractionR.destroy();
	diffractionG.fft(&filterFtG); diffractionG.destroy();
	diffractionB.fft(&filterFtB); diffractionB.destroy();
	
	filterFtR.multiply(&baseFt);
	filterFtG.multiply(&baseFt);
	filterFtB.multiply(&baseFt);
	baseFt.destroy();

	ImagePlane output;
	output.initialize(margins.width, margins.height, 0.f, 0.f);
	ComplexMap2D tempR, tempG, tempB;
	filterFtR.inverseFft(&tempR); filterFtR.destroy();
	filterFtG.inverseFft(&tempG); filterFtG.destroy();
	filterFtB.inverseFft(&tempB); filterFtB.destroy();
	for (int i = margins.left; i < margins.left + margins.width; i++) {
		for (int j = margins.top; j < margins.top + margins.height; j++) {
			math::Vector fragment = math::loadVector(
				tempR.get(i, j).r,
				tempG.get(i, j).r,
				tempB.get(i, j).r);
			output.set(fragment, i - margins.left, j - margins.top);
		}
	}

	std::cout << "Margins: " << margins.width << " " << margins.height << std::endl;

	//ImageByteBuffer safeBuffer;
	//output.fillByteBuffer(&safeBuffer);
	//output.destroy();
	
	//j.write(&safeBuffer, (std::string(TMP_PATH) + "safe.jpg").c_str());

	std::string fname = createUniqueRenderFilename("aperture_demo", samplesPerPixel);
	std::string imageFname = std::string(RENDER_OUTPUT) + "bitmap/" + fname + ".jpg";
	std::string rawFname = std::string(RENDER_OUTPUT) + "raw/" + fname + ".fpm";

	RawFile rawFile;
	rawFile.writeRawFile(rawFname.c_str(), &sceneBuffer);

	sceneBuffer.add(&output); output.destroy();
	sceneBuffer.applyGammaCurve((math::real)(1.0 / 2.2));
	writeJpeg(imageFname.c_str(), &sceneBuffer, 95);

	sceneBuffer.destroy();
	planeWaveApproximation.destroy();
	rayTracer.destroy();
	//aperture.destroy();

	kdtree.destroy();
}
