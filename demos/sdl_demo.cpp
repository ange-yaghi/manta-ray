#include <demos.h>

#include <sdl_compiler.h>
#include <simple_bsdf_material_node.h>
#include <sdl_node.h>
#include <sdl_compilation_unit.h>
#include <sdl_compilation_error.h>
#include <node_program.h>
#include <path.h>

using namespace manta;

void manta_demo::sdlDemo(int samplesPerPixel, int resolutionX, int resolutionY) {
	// Top-level parameters
	constexpr bool USE_ACCELERATION_STRUCTURE = true;
	constexpr bool DETERMINISTIC_SEED_MODE = false;
	constexpr bool TRACE_SINGLE_PIXEL = false;
	constexpr bool WRITE_KDTREE_TO_FILE = false;
	constexpr bool LENS_SIMULATION = true;
	constexpr bool POLYGON_APERTURE = true;

	Scene scene;
	NodeProgram program;
	RayTracer rayTracer;
	rayTracer.setMaterialManager(program.getMaterialManager());

	// Create all materials
	SdlCompiler compiler;
	compiler.addSearchPath(SDL_LIB_PATH);
	SdlCompilationUnit *unit = compiler.compile(SDL_PATH "sdl_demo.mr");
	const SdlErrorList *errors = compiler.getErrorList();

	unit->build(&program);
	program.execute();

	LambertianBSDF lambert;
	SimpleBSDFMaterial *defaultMaterial = rayTracer.getMaterialManager()->newMaterial<SimpleBSDFMaterial>();
	defaultMaterial->setName("Default");
	defaultMaterial->setBSDF(&lambert);

	// Create all scene geometry
	Mesh *boxCity = static_cast<ObjFileNode *>(program.getNode(1))->getMesh();
	KDTree *boxKdTree = static_cast<KdTreeNode *>(program.getNode(2))->getKdTree();

	SpherePrimitive outdoorTopLightGeometry;
	outdoorTopLightGeometry.setRadius((math::real)10.0);
	outdoorTopLightGeometry.setPosition(math::loadVector(20.f, 30.0f, -13.5f));

	if (WRITE_KDTREE_TO_FILE) {
		boxKdTree->writeToObjFile("../../workspace/test_results/box_city_kdtree.obj");
	}

	SceneObject *boxCityObject = scene.createSceneObject();
	if (USE_ACCELERATION_STRUCTURE) boxCityObject->setGeometry(boxKdTree);
	else boxCityObject->setGeometry(boxCity);
	boxCityObject->setDefaultMaterial(defaultMaterial);

	SceneObject *outdoorTopLightObject = scene.createSceneObject();
	outdoorTopLightObject->setGeometry(&outdoorTopLightGeometry);
	outdoorTopLightObject->setDefaultMaterial(defaultMaterial);

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
	polygonalAperture.setBladeCurvature(0.25f);
	polygonalAperture.initialize(8);

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
	std::string fraunFname = std::string(RENDER_OUTPUT) + "bitmap/" + fname + "_fraun" + ".jpg";
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
	rayTracer.setBackgroundColor(math::loadScalar(1.1f));
	rayTracer.setPathRecordingOutputDirectory("../../workspace/diagnostics/");
	rayTracer.setDeterministicSeedMode(DETERMINISTIC_SEED_MODE);

	if (TRACE_SINGLE_PIXEL) {
		rayTracer.tracePixel(915, 985, &scene, group, &sceneBuffer);
	}
	else {
		rayTracer.traceAll(&scene, group, &sceneBuffer);
	}

	// Clean up the camera
	delete group;

	RawFile rawFile;
	rawFile.writeRawFile(rawFname.c_str(), &sceneBuffer);

	writeJpeg(imageFname.c_str(), &sceneBuffer, 95);

	sceneBuffer.destroy();
	rayTracer.destroy();

	boxCity->destroy();
	//boxCityObj.destroy();
	//kdtree.destroy();
	polygonalAperture.destroy();

	std::cout << "Standard allocator memory leaks:     " << StandardAllocator::Global()->getLedger() << ", " << StandardAllocator::Global()->getCurrentUsage() << std::endl;
}
