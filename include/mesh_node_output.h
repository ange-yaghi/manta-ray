#ifndef MESH_NODE_OUTPUT_H
#define MESH_NODE_OUTPUT_H

#include <node_output.h>

#include <mesh.h>

namespace manta {

	class MeshNodeOutput : public NodeOutput {
	public:
		static const NodeType MeshNodeType;

	public:
		MeshNodeOutput();
		virtual ~MeshNodeOutput();

		Mesh *getMesh() const { return m_mesh; }
		void setMesh(Mesh *mesh) { m_mesh = mesh; }

	protected:
		Mesh *m_mesh;
	};

} /* namespace manta */

#endif /* MESH_NODE_OUTPUT_H */
