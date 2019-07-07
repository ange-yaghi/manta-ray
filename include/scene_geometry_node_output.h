#ifndef SCENE_GEOMETRY_NODE_OUTPUT_H
#define SCENE_GEOMETRY_NODE_OUTPUT_H

#include <node_output.h>

namespace manta {

	class SceneGeometry;

	class SceneGeometryNodeOutput : public NodeOutput {
	public:
		static const NodeType SceneGeometryNodeType;

	public:
		SceneGeometryNodeOutput();
		SceneGeometryNodeOutput(const NodeType *nodeType);
		virtual ~SceneGeometryNodeOutput();

		SceneGeometry *getSceneGeometry() const { return m_sceneGeometry; }
		void setSceneGeometry(SceneGeometry *geometry) { m_sceneGeometry = geometry; }

	protected:
		SceneGeometry *m_sceneGeometry;
	};

} /* namespace manta */

#endif /* SCENE_OBJECT_NODE_OUTPUT_H */
