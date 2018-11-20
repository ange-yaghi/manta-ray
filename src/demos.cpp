#include <demos.h>

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
#include <memory_management.h>
#include <mesh.h>
#include <obj_file_loader.h>
#include <manta_math.h>

manta::math::Vector getColor(int r, int g, int b) {
	return manta::math::loadVector(r / 255.0f, g / 255.0f, b / 255.0f);
}

void manta::simpleRoomDemo(int samplesPerPixel, int resolutionX, int resolutionY) {
	Scene scene;

	// Load all object files
	ObjFileLoader smallHouseObj;
	bool result = smallHouseObj.readObjFile("../../models/small_house.obj");

	ObjFileLoader tableObj;
	result = tableObj.readObjFile("../../models/table.obj");

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

	SimpleSpecularDiffuseMaterial tableMaterial;
	tableMaterial.setEmission(math::constants::Zero);
	tableMaterial.setDiffuseColor(getColor(78, 46, 40));
	tableMaterial.setSpecularColor(getColor(100, 100, 100));

	SimpleSpecularDiffuseMaterial groundMaterial;
	groundMaterial.setEmission(math::constants::Zero);
	groundMaterial.setDiffuseColor(math::mul(math::loadVector(78, 46, 40), math::loadScalar(0.001)));
	groundMaterial.setSpecularColor(math::constants::Zero);

	// Create all scene geometry
	Mesh smallHouse;
	smallHouse.initialize(smallHouseObj.getFaceCount(), smallHouseObj.getVertexCount());

	Face *tFaces = smallHouse.getFaces();
	SimpleVertex *tVerts = smallHouse.getVertices();

	for (unsigned int i = 0; i < smallHouseObj.getFaceCount(); i++) {
		ObjFace *face = smallHouseObj.getFace(i);
		tFaces[i].u = face->v1 - 1;
		tFaces[i].v = face->v2 - 1;
		tFaces[i].w = face->v3 - 1;
	}

	for (unsigned int i = 0; i < smallHouseObj.getVertexCount(); i++) {
		math::Vector3 *v = smallHouseObj.getVertex(i);
		tVerts[i].location = math::loadVector(*v);
	}
	smallHouse.precomputeValues();
	smallHouse.setFastIntersectEnabled(false);
	smallHouse.setFastIntersectRadius((math::real)4.0);

	Mesh table;
	table.initialize(tableObj.getFaceCount(), tableObj.getVertexCount());

	tFaces = table.getFaces();
	tVerts = table.getVertices();

	for (unsigned int i = 0; i < tableObj.getFaceCount(); i++) {
		ObjFace *face = tableObj.getFace(i);
		tFaces[i].u = face->v1 - 1;
		tFaces[i].v = face->v2 - 1;
		tFaces[i].w = face->v3 - 1;
	}

	for (unsigned int i = 0; i < tableObj.getVertexCount(); i++) {
		math::Vector3 *v = tableObj.getVertex(i);
		tVerts[i].location = math::loadVector(*v);
	}
	table.precomputeValues();
	table.setFastIntersectEnabled(false);
	table.setFastIntersectRadius((math::real)4.0);

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
	SceneObject *smallHouseObject = scene.createSceneObject();
	smallHouseObject->setGeometry(&smallHouse);
	smallHouseObject->setMaterial(&wallMaterial);

	SceneObject *tableObject = scene.createSceneObject();
	tableObject->setGeometry(&table);
	tableObject->setMaterial(&tableMaterial);

	SceneObject *ground = scene.createSceneObject();
	ground->setGeometry(&groundGeometry);
	ground->setMaterial(&groundMaterial);

	SceneObject *outdoorTopLightObject = scene.createSceneObject();
	outdoorTopLightObject->setGeometry(&outdoorTopLightGeometry);
	outdoorTopLightObject->setMaterial(&outdoorTopLightMaterial);

	SceneObject *lightSource = scene.createSceneObject();
	lightSource->setGeometry(&outdoorLightGeometry);
	lightSource->setMaterial(&outdoorLight);

	// Create the camera
	CameraRayEmitterGroup camera;
	camera.setSamplingWidth(2);
	camera.setDirection(math::loadVector(-1.0, 0.0, 0.0));
	camera.setPosition(math::loadVector(5.0, 2.0, 0.0));
	camera.setUp(math::loadVector(0.0f, 1.0, 0.0));
	camera.setPlaneDistance(1.0f);
	camera.setPlaneHeight(1.0f);
	camera.setResolutionX(resolutionX);
	camera.setResolutionY(resolutionY);
	camera.setSamplesPerPixel(samplesPerPixel);
	
	// Create the raytracer
	RayTracer rayTracer;
	rayTracer.initialize(500 * MB, 100 * MB, 12, 10000, true);
	rayTracer.setBackgroundColor(getColor(135, 206, 235));
	rayTracer.traceAll(&scene, &camera);

	// Output the results to file
	math::Vector *pixels = (math::Vector *)_aligned_malloc(sizeof(math::Vector) * resolutionX * resolutionY, 16);

	for (int i = 0; i < resolutionY; i++) {
		for (int j = 0; j < resolutionX; j++) {
			math::Vector v = math::constants::Zero;
			
			if (camera.getEmitters()[i * resolutionX + j] != nullptr) {
				v = ((CameraRayEmitter *)(camera.getEmitters()[i * resolutionX + j]))->getIntensity();
				math::real r = math::getX(v);
				math::real g = math::getY(v);
				math::real b = math::getZ(v);
			}

			pixels[i * resolutionX + j] = v;
		}
	}

	// Clean everything up
	for (int i = camera.getEmitterCount() - 1; i >= 0; i--) {
		if (camera.getEmitters()[i] != nullptr) {
			((CameraRayEmitter *)(camera.getEmitters()[i]))->destroyRays();
		}
	}

	SaveImageData(pixels, resolutionX, resolutionY, "test.bmp");
	camera.destroyEmitters();

	rayTracer.destroy();
}

void manta::teapotDemo(int samplesPerPixel, int resolutionX, int resolutionY) {
	Scene scene;

	// Load all object files
	ObjFileLoader smallHouseObj;
	bool result = smallHouseObj.readObjFile("../../models/small_house.obj");

	ObjFileLoader teapotObj;
	result = teapotObj.readObjFile("../../models/teapot.obj");

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

	SimpleSpecularDiffuseMaterial tableMaterial;
	tableMaterial.setEmission(math::constants::Zero);
	tableMaterial.setDiffuseColor(getColor(78, 46, 40));
	tableMaterial.setSpecularColor(getColor(100, 100, 100));

	SimpleSpecularDiffuseMaterial groundMaterial;
	groundMaterial.setEmission(math::constants::Zero);
	groundMaterial.setDiffuseColor(math::mul(math::loadVector(78, 46, 40), math::loadScalar(0.001)));
	groundMaterial.setSpecularColor(math::constants::Zero);

	// Create all scene geometry
	Mesh smallHouse;
	smallHouse.initialize(smallHouseObj.getFaceCount(), smallHouseObj.getVertexCount());

	Face *tFaces = smallHouse.getFaces();
	SimpleVertex *tVerts = smallHouse.getVertices();

	for (unsigned int i = 0; i < smallHouseObj.getFaceCount(); i++) {
		ObjFace *face = smallHouseObj.getFace(i);
		tFaces[i].u = face->v1 - 1;
		tFaces[i].v = face->v2 - 1;
		tFaces[i].w = face->v3 - 1;
	}

	for (unsigned int i = 0; i < smallHouseObj.getVertexCount(); i++) {
		math::Vector3 *v = smallHouseObj.getVertex(i);
		tVerts[i].location = math::loadVector(*v);
	}
	smallHouse.precomputeValues();
	smallHouse.setFastIntersectEnabled(false);
	smallHouse.setFastIntersectRadius((math::real)4.0);

	Mesh table;
	table.initialize(teapotObj.getFaceCount(), teapotObj.getVertexCount());

	tFaces = table.getFaces();
	tVerts = table.getVertices();

	for (unsigned int i = 0; i < teapotObj.getFaceCount(); i++) {
		ObjFace *face = teapotObj.getFace(i);
		tFaces[i].u = face->v1 - 1;
		tFaces[i].v = face->v2 - 1;
		tFaces[i].w = face->v3 - 1;
	}

	for (unsigned int i = 0; i < teapotObj.getVertexCount(); i++) {
		math::Vector3 *v = teapotObj.getVertex(i);
		tVerts[i].location = math::loadVector(*v);
	}
	table.precomputeValues();
	table.setFastIntersectEnabled(false);
	table.setFastIntersectRadius((math::real)4.0);

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
	SceneObject *smallHouseObject = scene.createSceneObject();
	smallHouseObject->setGeometry(&smallHouse);
	smallHouseObject->setMaterial(&wallMaterial);

	SceneObject *tableObject = scene.createSceneObject();
	tableObject->setGeometry(&table);
	tableObject->setMaterial(&tableMaterial);

	SceneObject *ground = scene.createSceneObject();
	ground->setGeometry(&groundGeometry);
	ground->setMaterial(&groundMaterial);

	SceneObject *outdoorTopLightObject = scene.createSceneObject();
	outdoorTopLightObject->setGeometry(&outdoorTopLightGeometry);
	outdoorTopLightObject->setMaterial(&outdoorTopLightMaterial);

	SceneObject *lightSource = scene.createSceneObject();
	lightSource->setGeometry(&outdoorLightGeometry);
	lightSource->setMaterial(&outdoorLight);

	// Create the camera
	CameraRayEmitterGroup camera;
	camera.setSamplingWidth(2);
	camera.setDirection(math::loadVector(-1.0, 0.0, 0.0));
	camera.setPosition(math::loadVector(5.0, 2.0, 0.0));
	camera.setUp(math::loadVector(0.0f, 1.0, 0.0));
	camera.setPlaneDistance(1.0f);
	camera.setPlaneHeight(1.0f);
	camera.setResolutionX(resolutionX);
	camera.setResolutionY(resolutionY);
	camera.setSamplesPerPixel(samplesPerPixel);

	// Create the raytracer
	RayTracer rayTracer;
	rayTracer.initialize(500 * MB, 50 * MB, 12, 10000, true);
	rayTracer.setBackgroundColor(getColor(135, 206, 235));
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

	SaveImageData(pixels, resolutionX, resolutionY, "test.bmp");
	camera.destroyEmitters();

	rayTracer.destroy();
}
