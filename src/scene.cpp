#include "../include/scene.h"

#include "../include/scene_object.h"

#include "../include/area_light.h"

manta::Scene::Scene() {
    // temp
    AreaLight *newAreaLight = new AreaLight();
    newAreaLight->setDirection(math::loadVector(0, -1, 0));
    newAreaLight->setHeight(1.0f);
    newAreaLight->setWidth(1.0);
    newAreaLight->setIntensity(math::loadVector(1, 1, 1));
    newAreaLight->setOrigin(math::loadVector(0, 2, 0));
    newAreaLight->setUp(math::loadVector(1, 0, 0));

    addLight(newAreaLight);
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
