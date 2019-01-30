#include <scene.h>

#include <scene_object.h>

manta::Scene::Scene() {

}

manta::Scene::~Scene() {

}

manta::SceneObject *manta::Scene::createSceneObject() {
	SceneObject *newObject = new SceneObject;
	m_sceneObjects.push_back(newObject);

	return newObject;
}
