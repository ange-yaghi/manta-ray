#ifndef MANTARAY_OBJ_FILE_NODE_H
#define MANTARAY_OBJ_FILE_NODE_H

#include "object_reference_node.h"

namespace manta {

    class Mesh;

	class ObjFileNode : public ObjectReferenceNode<Mesh> {
	public:
		ObjFileNode() : ObjectReferenceNode<Mesh>()
		virtual ~ObjFileNode();

	protected:
        virtual void _evaluate() {
            std::string filename;
            m_filename->fullCompute((void *)&filename);

            ObjFileLoader loader;
            loader.loadObjFile(filename.c_str());

            Mesh *mesh = new Mesh;
            // TODO
            //mesh->loadObjFileData(&loader, getProgram()->getMaterialManager());

            m_output.setReference(mesh);
        }

        virtual void _destroy() {
            /* void */
        }

        virtual void registerInputs() {
            registerInput(&m_filename, "filename");
        }

	protected:
		piranha::pNodeInput m_filename;
	};

} /* namespace manta */

#endif /* MANTARAY_OBJ_FILE_NODE_H */


void manta::ObjFileNode::_evaluate() {
    std::string filename;
    m_filename->fullCompute((void *)&filename);

    ObjFileLoader loader;
    loader.loadObjFile(filename.c_str());

    Mesh *mesh = new Mesh;
    // TODO
    //mesh->loadObjFileData(&loader, getProgram()->getMaterialManager());

    m_output.setMesh(mesh);
    m_mesh = mesh;
}

void manta::ObjFileNode::_destroy() {
    /* void */
}

void manta::ObjFileNode::registerOutputs() {

}

void manta::ObjFileNode::registerInputs() {
    registerInput(&m_filename, "filename");
}