#include <iostream>

#include <light_ray.h>
#include <sphere_primitive.h>
#include <intersection_point.h>
#include <scene.h>
#include <scene_object.h>
#include <scene_geometry.h>
#include <material.h>
#include <simple_diffuse_material.h>
#include <simple_specular_diffuse_material.h>
#include <ray_tracer.h>
#include <camera_ray_emitter_group.h>
#include <camera_ray_emitter.h>
#include <image_handling.h>

#include <manta_math.h>

using namespace manta;

math::Vector getColor(int r, int g, int b) {
	return math::loadVector(r / 255.0f, g / 255.0f, b / 255.0f);
}

int main() {
	Scene scene;

	SimpleSpecularDiffuseMaterial leftWallMaterial;
	SimpleSpecularDiffuseMaterial rightWallMaterial;
	SimpleSpecularDiffuseMaterial backWallMaterial;
	SimpleSpecularDiffuseMaterial sphere1Material;
	SimpleSpecularDiffuseMaterial sphere2Material;
	SimpleSpecularDiffuseMaterial lightMaterial;
	SimpleSpecularDiffuseMaterial floorMaterial;

	leftWallMaterial.setEmission(math::loadVector(0.0f, 0.0f, 0.0f));
	leftWallMaterial.setDiffuseColor(getColor(0xf7, 0xd0, 0x8a));
	leftWallMaterial.setSpecularColor(getColor(10, 10, 10));

	rightWallMaterial.setEmission(math::loadVector(0.0f, 0.0f, 0.0f));
	rightWallMaterial.setDiffuseColor(getColor(0x3F, 0x51, 0xB5));
	rightWallMaterial.setDiffuseColor(getColor(0x87, 0xB6, 0xA7));
	rightWallMaterial.setDiffuseColor(getColor(0x40, 0xC4, 0xFF));
	rightWallMaterial.setSpecularColor(getColor(10, 10, 10));

	backWallMaterial.setEmission(math::loadVector(0.0f, 0.0f, 0.0f));
	backWallMaterial.setDiffuseColor(getColor(200, 200, 200));
	backWallMaterial.setSpecularColor(getColor(10, 10, 10));

	sphere1Material.setEmission(math::loadVector(0.0f, 0.0f, 0.0f));
	sphere1Material.setDiffuseColor(getColor(228, 0, 25));
	sphere1Material.setSpecularColor(getColor(90, 90, 90));

	sphere2Material.setEmission(math::loadVector(0.0f, 0.0f, 0.0f));
	//sphere2Material.setDiffuseColor(getColor(0xe3, 0xf0, 0x9b));
	//sphere2Material.setSpecularColor(getColor(100, 100, 100));
	sphere2Material.setDiffuseColor(getColor(0, 0, 0));
	sphere2Material.setSpecularColor(getColor(0xFF, 0xFF, 0xFF));

	lightMaterial.setEmission(math::loadVector(7.2f, 7.2f, 7.2f));
	lightMaterial.setDiffuseColor(math::loadVector(1.0f, 1.0f, 1.0f));
	lightMaterial.setSpecularColor(getColor(0, 0, 0));

	floorMaterial.setEmission(math::loadVector(0.0f, 0.0f, 0.0f));
	floorMaterial.setDiffuseColor(getColor(200, 200, 200));
	floorMaterial.setSpecularColor(getColor(10, 10, 10));

	const float sphereSize = 10000.0f;
	const float roomSize = 5.0f;

	SpherePrimitive backWall;
	backWall.setRadius(sphereSize);
	backWall.setPosition(math::loadVector(-sphereSize-roomSize, 0.0f, 0.0f));

	SpherePrimitive leftWall;
	leftWall.setRadius(sphereSize);
	leftWall.setPosition(math::loadVector(0.0f, 0.0f, -sphereSize-roomSize));

	SpherePrimitive rightWall;
	rightWall.setRadius(sphereSize);
	rightWall.setPosition(math::loadVector(0.0f, 0.0f, sphereSize+roomSize));

	SpherePrimitive ceiling;
	ceiling.setRadius(sphereSize);
	ceiling.setPosition(math::loadVector(0.0f, sphereSize+roomSize*2, 0.0f));

	SpherePrimitive sphere;
	sphere.setRadius(1.0f);
	sphere.setPosition(math::loadVector(2.0f, 1.0f, 2.0f));

	SpherePrimitive sphere2;
	sphere2.setRadius(2.0f);
	sphere2.setPosition(math::loadVector(0.0f, 2.0f, -2.0f));

	SpherePrimitive floor;
	floor.setRadius(sphereSize);
	floor.setPosition(math::loadVector(0.0f, -sphereSize, 0.0f));

	SpherePrimitive light;
	light.setRadius(10.0f);
	light.setPosition(math::loadVector(0.0f, roomSize*2 + 9.75f, 0.0f));

	SceneObject *backWallObject = scene.createSceneObject();
	backWallObject->setMaterial(&backWallMaterial);
	backWallObject->setGeometry(&backWall);

	SceneObject *rightWallObject = scene.createSceneObject();
	rightWallObject->setMaterial(&rightWallMaterial);
	rightWallObject->setGeometry(&rightWall);

	SceneObject *leftWallObject = scene.createSceneObject();
	leftWallObject->setMaterial(&leftWallMaterial);
	leftWallObject->setGeometry(&leftWall);

	SceneObject *lightObject = scene.createSceneObject();
	lightObject->setMaterial(&lightMaterial);
	lightObject->setGeometry(&light);

	SceneObject *sphereObject = scene.createSceneObject();
	sphereObject->setMaterial(&sphere1Material);
	sphereObject->setGeometry(&sphere);

	SceneObject *sphere2Object = scene.createSceneObject();
	sphere2Object->setMaterial(&sphere2Material);
	sphere2Object->setGeometry(&sphere2);

	SceneObject *floorObject = scene.createSceneObject();
	floorObject->setMaterial(&floorMaterial);
	floorObject->setGeometry(&floor);

	SceneObject *ceilingObject = scene.createSceneObject();
	ceilingObject->setMaterial(&floorMaterial);
	ceilingObject->setGeometry(&ceiling);

	int width = 1024;
	int height = 768;

	width = 100;
	height = 100;

	CameraRayEmitterGroup camera;
	camera.setSamplingWidth(2);
	camera.setDirection(math::loadVector(-1.0f, 0.0f, 0.0f));
	camera.setPosition(math::loadVector(15.0f, roomSize, 0.0f));
	camera.setUp(math::loadVector(0.0f, 1.0f, 0.0f));
	camera.setPlaneDistance(1.0f);
	camera.setPlaneHeight(1.0f);
	camera.setResolutionX(width);
	camera.setResolutionY(height);
	camera.setSamplesPerPixel(1000);


	//CameraRayEmitter cameraEmitter;
	//cameraEmitter.setDegree(0);
	//cameraEmitter.setSamplesPerRay(1000);

	//cameraEmitter.generateRays();

	RayTracer rayTracer;
	rayTracer.traceAll(&scene, &camera);

	math::Vector *pixels = (math::Vector *)_aligned_malloc(sizeof(math::Vector) * width * height, 16);

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			math::Vector v = ((CameraRayEmitter *)(camera.getEmitters()[i * width + j]))->getIntensity();
			math::real r = math::getX(v);
			math::real g = math::getY(v);
			math::real b = math::getZ(v);

			if (r > 0.0 || g > 0.0 || b > 0.0) {
				//std::cout << "+";
			} 
			else {
				//std::cout << " ";
			}

			pixels[i * width + j] = v;
		}
		//std::cout << std::endl;
	}

	SaveImageData(pixels, width, height, "test.bmp");

	int a = 0;
}