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

manta::SceneAddLight::SceneAddLight() {
    m_sceneInput = nullptr;
    m_lightInput = nullptr;
}

manta::SceneAddLight::~SceneAddLight() {
    /* void */
}

void manta::SceneAddLight::_evaluate() {
    Scene *scene = getObject<Scene>(m_sceneInput);
    Light *light = getObject<Light>(m_lightInput);

    scene->addLight(light);

    setOutput(scene);
}

void manta::SceneAddLight::registerInputs() {
    registerInput(&m_sceneInput, "this");
    registerInput(&m_lightInput, "light");
}
