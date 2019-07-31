#include "../include/obj_file_node.h"

#include "../include/obj_file_loader.h"
#include "../include/mesh.h"
#include "../include/material_library.h"
#include "../include/path.h"

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

    Path resolvedPath;
    resolvePath(&Path(filename.c_str()), &resolvedPath);

    MaterialLibrary *library =
        static_cast<ObjectReferenceNodeOutput<MaterialLibrary> *>(m_materialLibrary)->getReference();

	ObjFileLoader loader;
	loader.loadObjFile(resolvedPath.toString().c_str());

	Mesh *mesh = new Mesh;
	mesh->loadObjFileData(&loader, library);

	m_output.setReference(mesh);
	m_mesh = mesh;
}

void manta::ObjFileNode::_destroy() {
	/* void */
}

void manta::ObjFileNode::registerInputs() {
	registerInput(&m_filename, "filename");
    registerInput(&m_materialLibrary, "materials");
}
