#ifndef MANTARAY_SCENE_GEOMETRY_NODE_OUTPUT_H
#define MANTARAY_SCENE_GEOMETRY_NODE_OUTPUT_H

#include <piranha.h>

namespace manta {

	class SceneGeometry;

	class SceneGeometryNodeOutput : public piranha::NodeOutput {
	public:
		static const piranha::ChannelType SceneGeometryNodeType;

	public:
		SceneGeometryNodeOutput();
        SceneGeometryNodeOutput(const piranha::ChannelType *nodeType);
		virtual ~SceneGeometryNodeOutput();

		SceneGeometry *getSceneGeometry() const { return m_sceneGeometry; }
		void setSceneGeometry(SceneGeometry *geometry) { m_sceneGeometry = geometry; }

	protected:
		SceneGeometry *m_sceneGeometry;
	};

} /* namespace manta */

#endif /* MANTARAY_SCENE_OBJECT_NODE_OUTPUT_H */
