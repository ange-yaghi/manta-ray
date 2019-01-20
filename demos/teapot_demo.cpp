#include <demos.h>

using namespace manta;

void manta_demo::teapotDemo(int samplesPerPixel, int resolutionX, int resolutionY) {
	Scene scene;

	// Load all object files
	ObjFileLoader smallHouseObj;
	bool result = smallHouseObj.readObjFile(MODEL_PATH "small_house.obj");

	ObjFileLoader teapotObj;
	result = teapotObj.readObjFile(MODEL_PATH "teapot.obj");

	// Create all materials
	SimpleSpecularDiffuseMaterial wallMaterial;
	wallMaterial.setEmission(math::constants::Zero);
	wallMaterial.setDiffuseColor(getColor(200, 200, 200));
	wallMaterial.setSpecularColor(math::constants::Zero);

	SimpleSpecularDiffuseMaterial outdoorLight;
	outdoorLight.setEmission(math::loadVector(9, 8, 8));
	outdoorLight.setDiffuseColor(math::constants::Zero);
	outdoorLight.setSpecularColor(math::constants::Zero);

	SimpleSpecularDiffuseMaterial outdoorTopLightMaterial;
	outdoorTopLightMaterial.setEmission(math::loadVector(10, 10, 11));
	outdoorTopLightMaterial.setDiffuseColor(math::constants::Zero);
	outdoorTopLightMaterial.setSpecularColor(math::constants::Zero);

	SimplePhongLambertMaterial teapotMaterial;
	teapotMaterial.setEmission(math::constants::Zero);
	teapotMaterial.getSpecularBSDF()->setPower((math::real)2.0);
	teapotMaterial.setDiffuseColor(getColor(1, 1, 1));
	teapotMaterial.setSpecularColor(getColor(100, 100, 100));

	SimpleSpecularDiffuseMaterial groundMaterial;
	groundMaterial.setEmission(math::constants::Zero);
	groundMaterial.setDiffuseColor(math::mul(math::loadVector(78, 46, 40), math::loadScalar(0.001)));
	groundMaterial.setSpecularColor(math::constants::Zero);

	// Create all scene geometry
	Mesh smallHouse;
	smallHouse.loadObjFileData(&smallHouseObj);
	smallHouse.setFastIntersectEnabled(false);
	smallHouse.setFastIntersectRadius((math::real)4.0);

	Mesh teapot;
	teapot.loadObjFileData(&teapotObj);
	teapot.setFastIntersectEnabled(false);
	teapot.setFastIntersectRadius((math::real)4.0);

	SpherePrimitive outdoorLightGeometry;
	outdoorLightGeometry.setRadius((math::real)10.0);
	outdoorLightGeometry.setPosition(math::loadVector(10.5, 0.0, -20.5));

	SpherePrimitive outdoorTopLightGeometry;
	outdoorTopLightGeometry.setRadius((math::real)10.0);
	outdoorTopLightGeometry.setPosition(math::loadVector(0.0, 25.0, 2));

	SpherePrimitive groundGeometry;
	groundGeometry.setRadius((math::real)50000.0);
	groundGeometry.setPosition(math::loadVector(0.0, -50000.1, 0));

	// Create scene objects
	//SceneObject *smallHouseObject = scene.createSceneObject();
	//smallHouseObject->setGeometry(&smallHouse);
	//smallHouseObject->setMaterial(&wallMaterial);

	SceneObject *teapotObject = scene.createSceneObject();
	teapotObject->setGeometry(&teapot);
	teapotObject->setDefaultMaterial(&teapotMaterial);

	Octree octree;
	octree.initialize(10.0, math::loadVector(0, 0, 0));
	octree.analyze(&teapot, 4);

	//SceneObject *ground = scene.createSceneObject();
	//ground->setGeometry(&groundGeometry);
	//ground->setMaterial(&groundMaterial);

	//SceneObject *outdoorTopLightObject = scene.createSceneObject();
	//outdoorTopLightObject->setGeometry(&outdoorTopLightGeometry);
	//outdoorTopLightObject->setMaterial(&outdoorTopLightMaterial);

	//SceneObject *lightSource = scene.createSceneObject();
	//lightSource->setGeometry(&outdoorLightGeometry);
	//lightSource->setMaterial(&outdoorLight);

	SimpleSampler simpleSampler;

	// Create the camera
	StandardCameraRayEmitterGroup camera;
	camera.setSampler(&simpleSampler);
	camera.setDirection(math::loadVector(-1.0, 0.0, 0.0));
	camera.setPosition(math::loadVector(5.0, 2.0, 0.0));
	camera.setUp(math::loadVector(0.0f, 1.0, 0.0));
	camera.setPlaneDistance(1.0f);
	camera.setPlaneHeight(1.0f);
	camera.setResolutionX(resolutionX);
	camera.setResolutionY(resolutionY);
	camera.setSampleCount(samplesPerPixel);

	// Create the raytracer
	RayTracer rayTracer;
	rayTracer.initialize(500 * MB, 50 * MB, 12, 10000, true);
	rayTracer.setBackgroundColor(getColor(135, 206, 235));
	rayTracer.setDeterministicSeedMode(true);
	//rayTracer.traceAll(&scene, &camera);
	rayTracer.tracePixel(817, 689, &scene, &camera);

	// Output the results to a scene buffer
	SceneBuffer sceneBuffer;
	camera.fillSceneBuffer(&sceneBuffer);

	// Clean up the camera
	camera.destroyRays();
	camera.destroyEmitters();

	std::string fname = createUniqueRenderFilename("teapot_demo", samplesPerPixel);
	std::string imageFname = std::string(RENDER_OUTPUT) + "bitmap/" + fname + ".bmp";
	std::string rawFname = std::string(RENDER_OUTPUT) + "raw/" + fname + ".fpm";

	RawFile rawFile;
	rawFile.writeRawFile(rawFname.c_str(), &sceneBuffer);
	editImage(&sceneBuffer, imageFname);

	sceneBuffer.destroy();
	rayTracer.destroy();
}
