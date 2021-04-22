#include "../include/material.h"

#include "../include/material_library.h"

manta::Material::Material() {
    m_name = "";
    m_index = -1;

    m_library = nullptr;
    m_nameInput = nullptr;
}

manta::Material::~Material() {
    /* void */
}

manta::math::Vector manta::Material::getEmission(const IntersectionPoint &ip) {
    return math::constants::Zero;
}

void manta::Material::_initialize() {
    /* void */
}

void manta::Material::_evaluate() {
    MaterialLibrary *library = getObject<MaterialLibrary>(m_library);
    library->addMaterial(this);

    piranha::native_string name;
    m_nameInput->fullCompute((void *)&name);
    m_name = name;

    setOutput(this);
}

void manta::Material::registerInputs() {
    registerInput(&m_library, "library", true);
    registerInput(&m_nameInput, "name");
}
