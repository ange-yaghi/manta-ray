#include "../include/scene.h"

#include "../include/scene_object.h"

manta::Scene::Scene() {
    /* void */
}

manta::Scene::~Scene() {
    /* void */
}

manta::SceneObject *manta::Scene::createSceneObject() {
    SceneObject *newObject = new SceneObject;
    m_sceneObjects.push_back(newObject);

    return newObject;
}

void manta::Scene::addSceneObject(SceneObject *object) {
    m_sceneObjects.push_back(object);
}

void manta::Scene::_evaluate() {
    setOutput(this);
}
