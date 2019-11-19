#include "../include/material_library.h"

#include "../include/material.h"

manta::MaterialLibrary::MaterialLibrary() {
    m_currentIndex = 0;
}

manta::MaterialLibrary::~MaterialLibrary() {
    /* void */
}

void manta::MaterialLibrary::addMaterial(Material *material) {
    material->setIndex(m_currentIndex++);
    m_materials.push_back(material);
}

manta::Material *manta::MaterialLibrary::searchByName(const std::string &name) const {
    int materialCount = (int)m_materials.size();

    for (int i = 0; i < materialCount; i++) {
        if (m_materials[i]->getName() == name) {
            return m_materials[i];
        }
    }

    return nullptr;
}

void manta::MaterialLibrary::_evaluate() {
    setOutput(this);
}
