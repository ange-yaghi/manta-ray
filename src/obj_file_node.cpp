#include <obj_file_node.h>

#include <string_node_output.h>
#include <obj_file_loader.h>
#include <mesh.h>
#include <node_program.h>

#include <string>

manta::ObjFileNode::ObjFileNode() {
	/* void */
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

	ObjFileLoader loader;
	loader.loadObjFile(filename.c_str());

	Mesh *mesh = new Mesh;
	mesh->loadObjFileData(&loader, getProgram()->getMaterialManager());

	m_output.setMesh(mesh);
	m_mesh = mesh;
}

void manta::ObjFileNode::_destroy() {
	/* void */
}

void manta::ObjFileNode::registerOutputs() {
	setPrimaryOutput(&m_output);

	registerOutput(&m_output, "mesh");
}

void manta::ObjFileNode::registerInputs() {
	registerInput(&m_filename, "filename");
}
