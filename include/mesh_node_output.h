#ifndef MESH_NODE_OUTPUT_H
#define MESH_NODE_OUTPUT_H

#include <scene_geometry_node_output.h>

#include <mesh.h>

namespace manta {

	class MeshNodeOutput : public SceneGeometryNodeOutput {
	public:
		static const NodeType MeshNodeType;

	public:
		MeshNodeOutput();
		virtual ~MeshNodeOutput();

		Mesh *getMesh() const;
		void setMesh(Mesh *mesh);
	};

} /* namespace manta */

#endif /* MESH_NODE_OUTPUT_H */
