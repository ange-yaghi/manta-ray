#include <demos.h>

#include <iostream>

using namespace manta;

void manta_demo::penDemo(int samplesPerPixel, int resolutionX, int resolutionY) {
	// Top-level parameters
	constexpr bool LENS_SIMULATION = false;
	constexpr bool USE_ACCELERATION_STRUCTURE = true;
	constexpr bool DETERMINISTIC_SEED_MODE = false;
	constexpr bool TRACE_SINGLE_PIXEL = false;

	RayTracer rayTracer;
	Scene scene;

	// Load all object files
	ObjFileLoader penObj;
	bool result = penObj.readObjFile(MODEL_PATH "pen.obj");

	if (!result) {
		std::cout << "Could not open geometry file(s)" << std::endl;

		penObj.destroy();

		return;
	}
	
	// Create all materials
	PhongDistribution phongDist;
	phongDist.setPower(1000);

	PhongDistribution phongDist2;
	phongDist2.setPower(128);

	MicrofacetReflectionBSDF bsdf2;
	bsdf2.setDistribution(&phongDist);

	LambertianBSDF lambert;

	DielectricMediaInterface fresnel;
	fresnel.setIorIncident((math::real)1.0);
	fresnel.setIorTransmitted((math::real)1.5);

	BilayerBSDF paintBsdf;
	paintBsdf.setDiffuseMaterial(&lambert);
	paintBsdf.setCoatingDistribution(&phongDist);
	paintBsdf.setDiffuse(getColor(0xFC, 0xC2, 0x01));
	paintBsdf.setSpecularAtNormal(math::loadVector(0.0, 0.0, 0.0));

	BilayerBSDF chromeBSDF;
	chromeBSDF.setDiffuseMaterial(&lambert);
	chromeBSDF.setCoatingDistribution(&phongDist);
	chromeBSDF.setDiffuse(getColor(0, 0, 0));
	chromeBSDF.setSpecularAtNormal(math::loadVector(0.95, 0.95, 0.95));

	BilayerBSDF floorBSDF;
	floorBSDF.setDiffuseMaterial(&lambert);
	floorBSDF.setCoatingDistribution(&phongDist2);
	floorBSDF.setDiffuse(getColor(0x0, 0x0, 0x0));
	floorBSDF.setSpecularAtNormal(math::loadVector(0.5, 0.5, 0.5));

	SimpleBSDFMaterial *paintMaterial = rayTracer.getMaterialManager()->newMaterial<SimpleBSDFMaterial>();
	paintMaterial->setBSDF(&paintBsdf);
	paintMaterial->setEmission(math::constants::Zero);
	paintMaterial->setReflectance(getColor(0xff, 0xff, 0xff));
	paintMaterial->setName("PenBody");

	SimpleBSDFMaterial *chromeMaterial = rayTracer.getMaterialManager()->newMaterial<SimpleBSDFMaterial>();
	chromeMaterial->setBSDF(&chromeBSDF);
	chromeMaterial->setEmission(math::constants::Zero);
	chromeMaterial->setReflectance(getColor(0xff, 0xff, 0xff));
	chromeMaterial->setName("Chrome");

	SimpleBSDFMaterial *floorMaterial = rayTracer.getMaterialManager()->newMaterial<SimpleBSDFMaterial>();
	floorMaterial->setBSDF(&floorBSDF);
	floorMaterial->setEmission(math::constants::Zero);
	floorMaterial->setReflectance(getColor(0xff, 0xff, 0xff));
	floorMaterial->setName("Backdrop");

	SimpleBSDFMaterial outdoorTopLightMaterial;
	outdoorTopLightMaterial.setEmission(math::loadVector(2, 2, 2));
	outdoorTopLightMaterial.setReflectance(math::constants::Zero);

	// Create all scene geometry
	Mesh pen;
	pen.loadObjFileData(&penObj, rayTracer.getMaterialManager(), chromeMaterial->getIndex(), 0);

	// Destroy file loaders
	penObj.destroy();

	KDTree kdtree;
	kdtree.initialize(150, math::constants::Zero);
	kdtree.analyze(&pen, 4);

	math::real lightRadius = 10.0;

	SpherePrimitive light1Geometry;
	light1Geometry.setRadius(lightRadius);
	light1Geometry.setPosition(math::loadVector(19.45842, 12.42560, -13.78918));

	SpherePrimitive light2Geometry;
	light2Geometry.setRadius(lightRadius);
	light2Geometry.setPosition(math::loadVector(-20.255, 16.42560, -16.4256));

	SpherePrimitive light3Geometry;
	light3Geometry.setRadius(lightRadius);
	light3Geometry.setPosition(math::loadVector(3.28578, 12.42560, 17.577));

	// Create scene objects
	SceneObject *penObject = scene.createSceneObject();
	if (USE_ACCELERATION_STRUCTURE) {
		penObject->setGeometry(&kdtree);
	}
	else {
		penObject->setGeometry(&pen);
	}
	penObject->setDefaultMaterial(chromeMaterial);
	penObject->setName("Pen");

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

	math::Vector cameraPos = math::loadVector(9.436, 1.2, 4.5370);
	math::Vector target = math::loadVector(1.3, 0.35547, 0.0);

	math::Vector up = math::loadVector(0.0f, 1.0, 0.0);
	math::Vector dir = math::normalize(math::sub(target, cameraPos));
	up = math::cross(math::cross(dir, up), dir);
	up = math::normalize(up);

	cameraPos = math::sub(cameraPos, math::mul(dir, math::loadScalar(3.9))); // 3.9

	// Create the camera
	CameraRayEmitterGroup *group;
	manta::SimpleLens lens;
	lens.initialize();
	lens.setPosition(cameraPos);
	lens.setDirection(dir);
	lens.setUp(up);
	lens.setRadius(1.0);
	lens.setSensorResolutionX(resolutionX);
	lens.setSensorResolutionY(resolutionY);
	lens.setSensorHeight(25.0);
	lens.setSensorWidth(25.0 * (resolutionX / (math::real)resolutionY));
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
		aperture->setRadius((math::real)0.015);
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
		rayTracer.tracePixel(179, 1423, &scene, group);
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

	std::string fname = createUniqueRenderFilename("pen_demo", samplesPerPixel);
	std::string imageFname = std::string(RENDER_OUTPUT) + "bitmap/" + fname + ".bmp";
	std::string rawFname = std::string(RENDER_OUTPUT) + "raw/" + fname + ".fpm";

	RawFile rawFile;
	rawFile.writeRawFile(rawFname.c_str(), &sceneBuffer);

	sceneBuffer.applyGammaCurve((math::real)(1.0 / 2.2));
	manta::SaveImageData(sceneBuffer.getBuffer(), sceneBuffer.getWidth(), sceneBuffer.getHeight(), imageFname.c_str());

	sceneBuffer.destroy();
	rayTracer.destroy();
	pen.destroy();
	kdtree.destroy();

	std::cout << "Standard allocator memory leaks:     " << StandardAllocator::Global()->getLedger() << ", " << StandardAllocator::Global()->getCurrentUsage() << std::endl;
}
