#include "../include/obj_file_node.h"

#include "../include/obj_file_loader.h"
#include "../include/mesh.h"
#include "../include/material_library.h"
#include "../include/path.h"
#include "../include/material.h"

#include <piranha.h>
#include <string>

manta::ObjFileNode::ObjFileNode() : ObjectReferenceNode<Mesh>() {
    m_mesh = nullptr;
}

manta::ObjFileNode::~ObjFileNode() {
    /* void */
}

void manta::ObjFileNode::_initialize() {
    /* void */
}

void manta::ObjFileNode::_evaluate() {
    std::string filename;
    m_filename->fullCompute((void *)&filename);

    std::string defaultMaterial;
    m_defaultMaterial->fullCompute((void *)&defaultMaterial);

    Path resolvedPath;
    const Path filePath(filename.c_str());
    resolvePath(&filePath, &resolvedPath);

    MaterialLibrary *library = getObject<MaterialLibrary>(m_materialLibrary);

    ObjFileLoader loader;
    const bool result = loader.loadObjFile(resolvedPath.toString().c_str());
    if (!result) {
        throwError("Could not open .obj file: " + resolvedPath.toString());
        return;
    }

    int defaultMaterialIndex = -1;
    if (!defaultMaterial.empty()) {
        Material *defaultMaterialRef = library->searchByName(defaultMaterial);
        if (defaultMaterialRef != nullptr) {
            defaultMaterialIndex = defaultMaterialRef->getIndex();
        }
    }

    Mesh *mesh = new Mesh;
    mesh->loadObjFileData(&loader, library, defaultMaterialIndex);

    // Free memory used by object file
    loader.destroy();

    m_output.setReference(mesh);
    m_mesh = mesh;
}

void manta::ObjFileNode::_destroy() {
    if (m_mesh != nullptr) {
        m_mesh->destroy();
    }
}

void manta::ObjFileNode::registerInputs() {
    registerInput(&m_filename, "filename");
    registerInput(&m_materialLibrary, "materials");
    registerInput(&m_defaultMaterial, "default_material");
}
