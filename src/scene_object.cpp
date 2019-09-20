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
    m_scene = static_cast<ObjectReferenceNodeOutput<Scene> *>(m_sceneInput)->getReference();
    m_geometry = static_cast<ObjectReferenceNodeOutput<SceneGeometry> *>(m_geometryInput)->getReference();
    static_cast<piranha::NodeOutput *>(m_nameInput)->fullCompute((void *)&m_name);

    m_scene->addSceneObject(this);
    m_output.setReference(this);
}

void manta::SceneObject::registerInputs() {
    registerInput(&m_geometryInput, "geometry");
    registerInput(&m_nameInput, "name");
    registerInput(&m_sceneInput, "scene", true);
}
