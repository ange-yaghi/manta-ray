#include "../include/scene_geometry.h"

#include "../include/vector_node_output.h"
#include "../include/material_library.h"
#include "../include/material.h"

manta::SceneGeometry::SceneGeometry() {
    m_id = -1;

    m_materialsInput = nullptr;
    m_defaultMaterial = nullptr;
    m_positionInput = nullptr;

    m_defaultMaterialIndex = -1;
    m_library = nullptr;
}

manta::SceneGeometry::~SceneGeometry() {
    /* void */
}

int manta::SceneGeometry::getId() const {
    return m_id;
}

void manta::SceneGeometry::setId(int id) {
    m_id = id;
}

void manta::SceneGeometry::_initialize() {
    /* void */
}

void manta::SceneGeometry::_evaluate() {
    static_cast<VectorNodeOutput *>(m_positionInput)->sample(nullptr, (void *)&m_position);

    std::string defaultMaterial;
    m_defaultMaterial->fullCompute((void *)&defaultMaterial);

    m_library =
        static_cast<ObjectReferenceNodeOutput<MaterialLibrary> *>(m_materialsInput)->getReference();

    int defaultMaterialIndex = -1;
    if (!defaultMaterial.empty()) {
        Material *defaultMaterialRef = m_library->searchByName(defaultMaterial);
        if (defaultMaterialRef != nullptr) {
            defaultMaterialIndex = defaultMaterialRef->getIndex();
        }
    }

    m_defaultMaterialIndex = defaultMaterialIndex;

    setOutput(this);
}

void manta::SceneGeometry::registerInputs() {
    registerInput(&m_positionInput, "position");
    registerInput(&m_defaultMaterial, "default_material");
    registerInput(&m_materialsInput, "materials");
}
