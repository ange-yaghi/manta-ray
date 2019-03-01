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

	// Load all textures
	TextureNode texture;
	texture.loadFile(TEXTURE_PATH "/dark_wood.jpg", 2.2);

	TextureNode woodRoughness;
	woodRoughness.loadFile(TEXTURE_PATH "/wood_roughness.jpg", 1.0);

	TextureNode chromeRoughness;
	chromeRoughness.loadFile(TEXTURE_PATH "/chrome_roughness.jpg", 1.0);

	TextureNode floorWood;
	floorWood.loadFile(TEXTURE_PATH "/light_wood.jpg", 2.2);

	// Load all object files
	ObjFileLoader penObj;
	bool result = penObj.readObjFile(MODEL_PATH "pen.obj");

	if (!result) {
		std::cout << "Could not open geometry file(s)" << std::endl;
		penObj.destroy();
		return;
	}
	
	// Create all materials
	LambertianBSDF lambert;

	PhongDistribution chromeCoating;
	chromeCoating.setPower(10000);
	chromeCoating.setPowerNode(&chromeRoughness);
	chromeCoating.setMinMapPower((math::real)400.0);

	PhongDistribution woodCoating;
	woodCoating.setPower(1000);
	woodCoating.setPowerNode(&woodRoughness);
	woodCoating.setMinMapPower(2);

	PhongDistribution floorDistribution;
	floorDistribution.setPower(128);

	BilayerBSDF paintBsdf;
	paintBsdf.setDiffuseMaterial(&lambert);
	paintBsdf.setCoatingDistribution(&woodCoating);
	paintBsdf.setDiffuseNode(&texture);
	paintBsdf.setSpecularAtNormal(math::loadVector(0.0, 0.0, 0.0));

	BilayerBSDF chromeBSDF;
	chromeBSDF.setDiffuseMaterial(&lambert);
	chromeBSDF.setCoatingDistribution(&chromeCoating);
	chromeBSDF.setDiffuse(getColor(0, 0, 0));
	chromeBSDF.setSpecularAtNormal(math::loadVector(0.95, 0.95, 0.95));

	MicrofacetReflectionBSDF floorBSDF;
	floorBSDF.setDistribution(&floorDistribution);

	SimpleBSDFMaterial *paintMaterial = rayTracer.getMaterialManager()->newMaterial<SimpleBSDFMaterial>();
	paintMaterial->setBSDF(&paintBsdf);
	paintMaterial->setName("PenBody");

	SimpleBSDFMaterial *chromeMaterial = rayTracer.getMaterialManager()->newMaterial<SimpleBSDFMaterial>();
	chromeMaterial->setBSDF(&chromeBSDF);
	chromeMaterial->setName("Chrome");

	SimpleBSDFMaterial *floorMaterial = rayTracer.getMaterialManager()->newMaterial<SimpleBSDFMaterial>();
	floorMaterial->setBSDF(&floorBSDF);
	floorMaterial->setReflectanceNode(&floorWood);
	floorMaterial->setName("Backdrop");

	SimpleBSDFMaterial *strongLight = rayTracer.getMaterialManager()->newMaterial<SimpleBSDFMaterial>();
	strongLight->setEmission(math::loadVector(2.0, 2.0, 2.0));
	strongLight->setReflectance(math::constants::Zero);
	strongLight->setName("StrongLight");

	SimpleBSDFMaterial *weakLight = rayTracer.getMaterialManager()->newMaterial<SimpleBSDFMaterial>();
	weakLight->setEmission(math::loadVector(1.0, 1.0, 1.0));
	weakLight->setReflectance(math::constants::Zero);
	weakLight->setName("WeakLight");

	// Create all scene geometry
	Mesh pen;
	pen.loadObjFileData(&penObj, rayTracer.getMaterialManager(), chromeMaterial->getIndex(), 0);

	// Destroy file loaders
	penObj.destroy();

	KDTree kdtree;
	kdtree.initialize(150, math::constants::Zero);
	kdtree.analyze(&pen, 4);

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

	// Create the camera
	math::Vector cameraPos = math::loadVector(9.436, 1.2, 4.5370);
	math::Vector target = math::loadVector(1.3, 0.35547, 0.0);

	math::Vector up = math::loadVector(0.0f, 1.0, 0.0);
	math::Vector dir = math::normalize(math::sub(target, cameraPos));
	up = math::cross(math::cross(dir, up), dir);
	up = math::normalize(up);

	cameraPos = math::sub(cameraPos, math::mul(dir, math::loadScalar(3.9)));

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
