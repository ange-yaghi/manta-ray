#include "../include/obj_file_node.h"

#include "../include/obj_file_loader.h"
#include "../include/mesh.h"
#include "../include/material_library.h"
#include "../include/path.h"
#include "../include/material.h"
#include "../include/session.h"
#include "../include/console.h"

#include <piranha.h>
#include <string>

manta::ObjFileNode::ObjFileNode() : ObjectReferenceNode<Mesh>() {
    m_mesh = nullptr;
    m_materialLibrary = nullptr;

    m_filename = nullptr;
    m_defaultMaterial = nullptr;
    m_cacheKey = nullptr;
    m_overwriteCache = nullptr;
}

manta::ObjFileNode::~ObjFileNode() {
    /* void */
}

void manta::ObjFileNode::_initialize() {
    /* void */
}

void manta::ObjFileNode::_evaluate() {
    piranha::native_string filename;
    piranha::native_string defaultMaterial;
    piranha::native_string cacheKey;
    piranha::native_bool overrideCache;

    m_filename->fullCompute((void *)&filename);
    m_defaultMaterial->fullCompute((void *)&defaultMaterial);
    m_cacheKey->fullCompute((void *)&cacheKey);
    m_overwriteCache->fullCompute((void *)&overrideCache);

    Path resolvedPath;
    const Path filePath(filename.c_str());
    resolvePath(&filePath, &resolvedPath);

    MaterialLibrary *library = getObject<MaterialLibrary>(m_materialLibrary);

    int defaultMaterialIndex = -1;
    if (!defaultMaterial.empty()) {
        Material *defaultMaterialRef = library->searchByName(defaultMaterial);
        if (defaultMaterialRef != nullptr) {
            defaultMaterialIndex = defaultMaterialRef->getIndex();
        }
    }

    Mesh *mesh = Session::get().getCachedMesh(cacheKey);
    if (mesh == nullptr || overrideCache) {
        auto startTime = std::chrono::system_clock::now();
        manta::Session::get().getConsole()->out("Loading object file: " + resolvedPath.toString() + "\n");

        ObjFileLoader loader;
        const bool result = loader.loadObjFile(resolvedPath.toString().c_str());
        if (!result) {
            manta::Session::get().getConsole()->out("Error loading object file: " + resolvedPath.toString() + "\n");
            throwError("Could not open .obj file: " + resolvedPath.toString());
            return;
        }

        auto endTime = std::chrono::system_clock::now();
        std::chrono::duration<double> timeElapsed = endTime - startTime;
        std::stringstream ss;
        ss << "Loading took: " << timeElapsed.count() << "s" << "\n";

        manta::Session::get().getConsole()->out(ss.str());

        mesh = new Mesh;
        mesh->loadObjFileData(&loader);

        // Free memory used by object file
        loader.destroy();

        Session::get().putCachedMesh(cacheKey, mesh);
    }
    else {
        manta::Session::get().getConsole()->out("Using cached mesh data: " + cacheKey + "\n");
    }

    mesh->bindMaterialLibrary(library, defaultMaterialIndex);

    m_output.setReference(mesh);
    m_mesh = mesh;
}

void manta::ObjFileNode::_destroy() {
    /* void */
}

void manta::ObjFileNode::registerInputs() {
    registerInput(&m_filename, "filename");
    registerInput(&m_materialLibrary, "materials");
    registerInput(&m_defaultMaterial, "default_material");
    registerInput(&m_cacheKey, "cache_key");
    registerInput(&m_overwriteCache, "overwrite_cache");
}
