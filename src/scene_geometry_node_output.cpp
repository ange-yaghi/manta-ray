#include "../include/scene_geometry_node_output.h"

const piranha::ChannelType manta::SceneGeometryNodeOutput::SceneGeometryNodeType("SceneGeometryNodeType");

manta::SceneGeometryNodeOutput::SceneGeometryNodeOutput() 
	: NodeOutput(&SceneGeometryNodeType) 
{
	/* void */
}

manta::SceneGeometryNodeOutput::SceneGeometryNodeOutput(const piranha::ChannelType *nodeType) 
	: NodeOutput(nodeType) 
{
	/* void */
}

manta::SceneGeometryNodeOutput::~SceneGeometryNodeOutput() {
	/* void */
}
