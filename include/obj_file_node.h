#ifndef MANTARAY_OBJ_FILE_NODE_H
#define MANTARAY_OBJ_FILE_NODE_H

#include "object_reference_node.h"

namespace manta {

	class Mesh;

	class ObjFileNode : public ObjectReferenceNode<Mesh> {
	public:
		ObjFileNode();
		virtual ~ObjFileNode();

		Mesh *getMesh() const { return m_mesh; }

	protected:
		virtual void _initialize();
		virtual void _evaluate();
		virtual void _destroy();

		virtual void registerInputs();

	protected:
		piranha::pNodeInput m_filename;
        piranha::pNodeInput m_materialLibrary;

		Mesh *m_mesh;
	};

} /* namespace manta */

#endif /* MANTARAY_OBJ_FILE_NODE_H */
