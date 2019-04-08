#include <demos.h>

using namespace manta;

void manta_demo::boxCityDemo(int samplesPerPixel, int resolutionX, int resolutionY) {
	// Top-level parameters
	constexpr bool USE_ACCELERATION_STRUCTURE = true;
	constexpr bool DETERMINISTIC_SEED_MODE = false;
	constexpr bool TRACE_SINGLE_PIXEL = false;
	constexpr bool WRITE_KDTREE_TO_FILE = false;
	constexpr bool LENS_SIMULATION = true;
	constexpr bool POLYGON_APERTURE = true;
	constexpr bool ENABLE_FRAUNHOFER_DIFFRACTION = true;

	Scene scene;
	RayTracer rayTracer;

	ObjFileLoader boxCityObj;
	bool result = boxCityObj.readObjFile(MODEL_PATH "box_city.obj");

	if (!result) {
		std::cout << "Could not open geometry file" << std::endl;

		boxCityObj.destroy();

		return;
	}

	// Create all materials
	LambertianBSDF lambert;

	PhongDistribution blockCoating;
	blockCoating.setPower((math::real)300);
	BilayerBSDF blockBSDF;
	blockBSDF.setCoatingDistribution(&blockCoating);
	blockBSDF.setDiffuseMaterial(&lambert);
	blockBSDF.setDiffuse(getColor(0x50, 0x50, 0x50)); // 0xf1, 0xc4, 0x0f
	blockBSDF.setSpecularAtNormal(math::loadVector(0.02f, 0.02f, 0.02f));
	SimpleBSDFMaterial *blockMaterial = rayTracer.getMaterialManager()->newMaterial<SimpleBSDFMaterial>();
	blockMaterial->setName("Block");
	blockMaterial->setBSDF(&blockBSDF);
	//blockMaterial->setReflectance(math::loadVector(0.01f, 0.01f, 0.01f));

	SimpleBSDFMaterial outdoorTopLightMaterial;
	outdoorTopLightMaterial.setEmission(math::loadVector(40.f, 40.f, 40.f));
	outdoorTopLightMaterial.setReflectance(math::constants::Zero);

	SimpleBSDFMaterial *groundMaterial = rayTracer.getMaterialManager()->newMaterial<SimpleBSDFMaterial>();
	groundMaterial->setName("Ground");
	//groundMaterial->setReflectance(math::loadVector(0.01f, 0.01f, 0.01f));
	groundMaterial->setBSDF(&lambert);

	SimpleBSDFMaterial *sunMaterial = rayTracer.getMaterialManager()->newMaterial<SimpleBSDFMaterial>();
	sunMaterial->setName("Sun");
	sunMaterial->setReflectance(math::loadVector(0.0f, 0.0f, 0.0f));
	sunMaterial->setEmission(math::loadVector(100.0f, 100.0f, 100.0f));
	sunMaterial->setBSDF(nullptr);

	// Create all scene geometry
	Mesh boxCity;
	boxCity.loadObjFileData(&boxCityObj, rayTracer.getMaterialManager());
	boxCity.setFastIntersectEnabled(false);
	//boxCity.findQuads();

	SpherePrimitive outdoorTopLightGeometry;
	outdoorTopLightGeometry.setRadius((math::real)1.0);
	outdoorTopLightGeometry.setPosition(math::loadVector(20.f, 30.0f, -13.5f));

	// Create scene objects
	KDTree kdtree;
	kdtree.initialize(100.0f, math::constants::Zero);
	kdtree.analyze(&boxCity, 2);

	if (WRITE_KDTREE_TO_FILE) {
		kdtree.writeToObjFile("../../workspace/test_results/box_city_kdtree.obj");
	}

	SceneObject *boxCityObject = scene.createSceneObject();
	if (USE_ACCELERATION_STRUCTURE) boxCityObject->setGeometry(&kdtree);
	else boxCityObject->setGeometry(&boxCity);
	boxCityObject->setDefaultMaterial(blockMaterial);

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

	// Easy camera position control
	cameraPos = math::sub(cameraPos, math::mul(dir, math::loadScalar(0.f)));

	manta::SimpleLens lens;
	manta::PolygonalAperture polygonalAperture;
	polygonalAperture.initialize(6);

	if (POLYGON_APERTURE) lens.setAperture(&polygonalAperture);
	lens.initialize();
	lens.setPosition(cameraPos);
	lens.setDirection(dir);
	lens.setUp(up);
	lens.setRadius(1.0f);
	lens.setSensorResolutionX(resolutionX);
	lens.setSensorResolutionY(resolutionY);
	lens.setSensorHeight(10.0f);
	lens.setSensorWidth(10.0f * (resolutionX / (math::real)resolutionY));
	lens.update();

	std::string fname = createUniqueRenderFilename("box_city_demo", samplesPerPixel);
	std::string imageFname = std::string(RENDER_OUTPUT) + "bitmap/" + fname + ".jpg";
	std::string convFname = std::string(RENDER_OUTPUT) + "bitmap/" + fname + "_conv" + ".jpg";
	std::string rawFname = std::string(RENDER_OUTPUT) + "raw/" + fname + ".fpm";

	RandomSampler sampler;

	if (!LENS_SIMULATION) {
		StandardCameraRayEmitterGroup *camera = new StandardCameraRayEmitterGroup;
		camera->setDirection(dir);
		camera->setPosition(cameraPos);
		camera->setUp(up);
		camera->setPlaneDistance(1.0f);
		camera->setPlaneHeight(1.0f);
		camera->setResolutionX(resolutionX);
		camera->setResolutionY(resolutionY);
		camera->setSampleCount(samplesPerPixel);
		camera->setSampler(&sampler);

		group = camera;
	}
	else {
		math::real lensHeight = 1.0f;
		math::real focusDistance = 22.0f;

		Aperture *aperture = lens.getAperture();
		aperture->setRadius((math::real)0.18);
		lens.setFocus(focusDistance);

		// Initialize diffraction model
		//FraunhoferDiffraction::Settings settings;
		//FraunhoferDiffraction::defaultSettings(&settings);
		//settings.maxSamples = 4096;
		//settings.frequencyMultiplier = 3;

		//aperture->setRadius((math::real)5.5);
		//lens.getDiffractionModel()->generate(aperture, 1024, 0.05f, &settings);
		//aperture->setRadius((math::real)0.18);

		//JpegWriter writer;
		//ImageByteBuffer b;
		//VectorMap2D temp;
		//temp.copy(lens.getDiffractionModel()->getDiffractionPattern());
		//temp.scale(math::loadScalar(100000.0f));
		//temp.fillByteBuffer(&b);
		//temp.destroy();
		//writer.write(&b, convFname.c_str());
		//b.free();

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

	// Initialize and run the ray tracer
	rayTracer.initialize(200 * MB, 50 * MB, 12, 100, true);
	rayTracer.setBackgroundColor(math::loadVector(1.0, 1.0, 1.0));
	rayTracer.setDeterministicSeedMode(DETERMINISTIC_SEED_MODE);
	
	if (TRACE_SINGLE_PIXEL) {
		rayTracer.tracePixel(779, 942, &scene, group, &sceneBuffer);
	}
	else {
		rayTracer.traceAll(&scene, group, &sceneBuffer);
	}

	// Clean up the camera
	delete group;

	RawFile rawFile;
	rawFile.writeRawFile(rawFname.c_str(), &sceneBuffer);

	if (ENABLE_FRAUNHOFER_DIFFRACTION) {
		ComplexMap2D imageMapR, imageMapG, imageMapB, imageMapRSafe, imageMapGSafe, imageMapBSafe;
		imageMapR.copy(&sceneBuffer, 0);
		imageMapG.copy(&sceneBuffer, 1);
		imageMapB.copy(&sceneBuffer, 2);

		Margins margins;
		imageMapR.resizeSafe(&imageMapRSafe, &margins); imageMapR.destroy();
		imageMapG.resizeSafe(&imageMapGSafe, &margins); imageMapG.destroy();
		imageMapB.resizeSafe(&imageMapBSafe, &margins); imageMapB.destroy();

		polygonalAperture.setRadius(5.5f);

		FraunhoferDiffraction testFraun;
		FraunhoferDiffraction::Settings settings;
		FraunhoferDiffraction::defaultSettings(&settings);
		settings.frequencyMultiplier = 3.0;
		settings.maxSamples = 4096;

		testFraun.generate(&polygonalAperture, imageMapRSafe.getWidth(), 0.15f, &settings);

		JpegWriter writer;
		ImageByteBuffer b;
		VectorMap2D temp;
		temp.copy(testFraun.getDiffractionPattern());
		temp.scale(math::loadScalar(100000.0f));
		temp.fillByteBuffer(&b);
		temp.destroy();
		writer.write(&b, convFname.c_str());
		b.free();

		ComplexMap2D diffractionR, diffractionG, diffractionB;
		diffractionR.initialize(imageMapRSafe.getWidth(), imageMapRSafe.getHeight());
		diffractionG.initialize(imageMapRSafe.getWidth(), imageMapRSafe.getHeight());
		diffractionB.initialize(imageMapRSafe.getWidth(), imageMapRSafe.getHeight());
		for (int i = 0; i < imageMapRSafe.getWidth(); i++) {
			for (int j = 0; j < imageMapRSafe.getHeight(); j++) {
				diffractionR.set(math::Complex(math::getX(testFraun.getDiffractionPattern()->get(i, j)), 0.0f), i, j);
				diffractionG.set(math::Complex(math::getY(testFraun.getDiffractionPattern()->get(i, j)), 0.0f), i, j);
				diffractionB.set(math::Complex(math::getZ(testFraun.getDiffractionPattern()->get(i, j)), 0.0f), i, j);
			}
		}

		ComplexMap2D baseFtR, baseFtG, baseFtB, filterFtR, filterFtG, filterFtB;
		imageMapRSafe.fft(&baseFtR); imageMapRSafe.destroy();
		imageMapGSafe.fft(&baseFtG); imageMapGSafe.destroy();
		imageMapBSafe.fft(&baseFtB); imageMapBSafe.destroy();
		diffractionR.fft(&filterFtR); diffractionR.destroy();
		diffractionG.fft(&filterFtG); diffractionG.destroy();
		diffractionB.fft(&filterFtB); diffractionB.destroy();

		filterFtR.multiply(&baseFtR); baseFtR.destroy();
		filterFtG.multiply(&baseFtG); baseFtG.destroy();
		filterFtB.multiply(&baseFtB); baseFtB.destroy();

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

		output.applyGammaCurve((math::real)(1.0 / 2.2));
		writeJpeg(convFname.c_str(), &output, 95);
		output.destroy();
	}

	sceneBuffer.applyGammaCurve((math::real)(1.0 / 2.2));
	writeJpeg(imageFname.c_str(), &sceneBuffer, 95);

	sceneBuffer.destroy();
	rayTracer.destroy();

	boxCity.destroy();
	boxCityObj.destroy();
	kdtree.destroy();
	polygonalAperture.destroy();

	std::cout << "Standard allocator memory leaks:     " << StandardAllocator::Global()->getLedger() << ", " << StandardAllocator::Global()->getCurrentUsage() << std::endl;
}
