#include <scene_geometry_node_output.h>

const manta::NodeType manta::SceneGeometryNodeOutput::SceneGeometryNodeType("SceneGeometryNodeType");

manta::SceneGeometryNodeOutput::SceneGeometryNodeOutput() 
	: NodeOutput(&SceneGeometryNodeType) 
{
	/* void */
}

manta::SceneGeometryNodeOutput::SceneGeometryNodeOutput(const NodeType *nodeType) 
	: NodeOutput(nodeType) 
{
	/* void */
}

manta::SceneGeometryNodeOutput::~SceneGeometryNodeOutput() {
	/* void */
}
