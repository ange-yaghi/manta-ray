#ifndef MANTARAY_OBJ_FILE_NODE_H
#define MANTARAY_OBJ_FILE_NODE_H

#include <piranha.h>

#include "mesh_node_output.h"

namespace manta {

	class ObjFileNode : public piranha::Node {
	public:
		ObjFileNode();
		virtual ~ObjFileNode();

		virtual bool requiresMaterials() const { return true; }

		Mesh *getMesh() { return m_mesh; }

	protected:
		virtual void _initialize();
		virtual void _evaluate();
		virtual void _destroy();

		virtual void registerOutputs();
		virtual void registerInputs();

	protected:
		piranha::pNodeInput m_filename;
		MeshNodeOutput m_output;

	protected:
		Mesh *m_mesh;
	};

} /* namespace manta */

#endif /* MANTARAY_OBJ_FILE_NODE_H */
