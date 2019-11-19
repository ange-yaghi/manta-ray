#include "../include/scene_object.h"

#include "../include/scene.h"

manta::SceneObject::SceneObject() {
    m_name = "";
    m_defaultMaterial = nullptr;
    m_geometry = nullptr;
}

manta::SceneObject::~SceneObject() {
    /* void */
}

void manta::SceneObject::_evaluate() {
    m_scene = getObject<Scene>(m_sceneInput);
    m_geometry = getObject<SceneGeometry>(m_geometryInput);

    piranha::native_string name;
    m_nameInput->fullCompute((void *)&name);

    m_name = name;
    m_scene->addSceneObject(this);
    setOutput(this);
}

void manta::SceneObject::registerInputs() {
    registerInput(&m_geometryInput, "geometry");
    registerInput(&m_nameInput, "name");
    registerInput(&m_sceneInput, "scene", true);
}
