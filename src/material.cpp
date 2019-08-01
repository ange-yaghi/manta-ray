#include "../include/material.h"

#include "../include/material_library.h"

manta::Material::Material() {
    m_name = "";
    m_index = -1;

    m_output.setReference(this);
}

manta::Material::~Material() {
    /* void */
}

void manta::Material::_initialize() {
    /* void */
}

void manta::Material::_evaluate() {
    MaterialLibrary *library =
        static_cast<ObjectReferenceNodeOutput<MaterialLibrary> *>(m_library)
            ->getReference();

    library->addMaterial(this);

    static_cast<piranha::NodeOutput *>(m_nameInput)->fullCompute((void *)&m_name);
}

void manta::Material::registerInputs() {
    registerInput(&m_library, "library", true);
    registerInput(&m_nameInput, "name");
}
