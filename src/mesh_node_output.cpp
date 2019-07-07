#include <mesh_node_output.h>

#include <scene_geometry_node_output.h>

const manta::NodeType manta::MeshNodeOutput::MeshNodeType(
	"MeshNodeType", 
	&SceneGeometryNodeOutput::SceneGeometryNodeType);

manta::MeshNodeOutput::MeshNodeOutput() : SceneGeometryNodeOutput(&MeshNodeType) {
	/* void */
}

manta::MeshNodeOutput::~MeshNodeOutput() {
	/* void */
}

manta::Mesh *manta::MeshNodeOutput::getMesh() const {
	return static_cast<Mesh *>(m_sceneGeometry);
}

void manta::MeshNodeOutput::setMesh(Mesh *mesh) {
	m_sceneGeometry = mesh;
}
