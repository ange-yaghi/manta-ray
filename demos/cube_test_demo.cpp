#include <demos.h>

using namespace manta;

void manta_demo::cubeTestDemo(int samplesPerPixel, int resolutionX, int resolutionY) {
	Scene scene;

	// Load all object files
	ObjFileLoader cubeObj;
	bool result = cubeObj.readObjFile(MODEL_PATH "cube.obj");

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

	SimpleSpecularDiffuseMaterial teapotMaterial;
	teapotMaterial.setEmission(math::constants::Zero);
	teapotMaterial.setDiffuseColor(getColor(150, 0, 0));
	teapotMaterial.setSpecularColor(getColor(100, 100, 100));

	SimpleSpecularDiffuseMaterial groundMaterial;
	groundMaterial.setEmission(math::constants::Zero);
	groundMaterial.setDiffuseColor(math::constants::Zero);
	groundMaterial.setSpecularColor(getColor(100, 100, 100));

	// Create all scene geometry
	Mesh cube;
	cube.loadObjFileData(&cubeObj);
	cube.setFastIntersectEnabled(false);
	cube.setFastIntersectRadius((math::real)4.0);

	// Create scene objects
	SceneObject *cubeObject = scene.createSceneObject();
	cubeObject->setGeometry(&cube);
	cubeObject->setMaterial(&wallMaterial);

	//SceneObject *outdoorTopLightObject = scene.createSceneObject();
	//outdoorTopLightObject->setGeometry(&outdoorTopLightGeometry);
	//outdoorTopLightObject->setMaterial(&outdoorTopLightMaterial);

	//SceneObject *lightSource = scene.createSceneObject();
	//lightSource->setGeometry(&outdoorLightGeometry);
	//lightSource->setMaterial(&outdoorLight);

	// Create the camera
	CameraRayEmitterGroup camera;
	camera.setSamplingWidth(1);
	camera.setDirection(math::loadVector(-1.0, 0.0, 0.0));
	camera.setPosition(math::loadVector(7.0, 2.0, 0.0));
	camera.setUp(math::loadVector(0.0f, 1.0, 0.0));
	camera.setPlaneDistance(1.0f);
	camera.setPlaneHeight(1.0f);
	camera.setResolutionX(resolutionX);
	camera.setResolutionY(resolutionY);
	camera.setSamplesPerPixel(samplesPerPixel);

	// Create the raytracer
	RayTracer rayTracer;
	rayTracer.initialize(500 * MB, 500 * MB, 12, 10000, true);
	rayTracer.setBackgroundColor(getColor(0, 0, 0));
	rayTracer.traceAll(&scene, &camera);

	// Output the results to file
	math::Vector *pixels = (math::Vector *)_aligned_malloc(sizeof(math::Vector) * resolutionX * resolutionY, 16);

	for (int i = 0; i < resolutionY; i++) {
		for (int j = 0; j < resolutionX; j++) {
			math::Vector v = ((CameraRayEmitter *)(camera.getEmitters()[i * resolutionX + j]))->getIntensity();
			math::real r = math::getX(v);
			math::real g = math::getY(v);
			math::real b = math::getZ(v);

			pixels[i * resolutionX + j] = v;
		}
	}

	// Clean everything up
	for (int i = camera.getEmitterCount() - 1; i >= 0; i--) {
		((CameraRayEmitter *)(camera.getEmitters()[i]))->destroyRays();
	}

	SaveImageData(pixels, resolutionX, resolutionY, createUniqueRenderFilename(RENDER_OUTPUT, "cube_test", samplesPerPixel).c_str());
	camera.destroyEmitters();

	rayTracer.destroy();
}
