#include <mesh_node_output.h>

const manta::NodeType manta::MeshNodeOutput::MeshNodeType("MeshNodeType");

manta::MeshNodeOutput::MeshNodeOutput() : NodeOutput(&MeshNodeType) {
	m_mesh = nullptr;
}

manta::MeshNodeOutput::~MeshNodeOutput() {
	/* void */
}
