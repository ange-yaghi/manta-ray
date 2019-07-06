#ifndef OBJ_FILE_NODE_H
#define OBJ_FILE_NODE_H

#include <node.h>

#include <mesh_node_output.h>

namespace manta {

	class ObjFileNode : public Node {
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
		pNodeInput m_filename;
		MeshNodeOutput m_output;

	protected:
		Mesh *m_mesh;
	};

} /* namespace manta */

#endif /* OBJ_FILE_NODE_H */
