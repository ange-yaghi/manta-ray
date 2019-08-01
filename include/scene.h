#ifndef MANTARAY_SCENE_H
#define MANTARAY_SCENE_H

#include "object_reference_node.h"

#include <vector>

namespace manta {

	class SceneObject;

	class Scene : public ObjectReferenceNode<Scene> {
	public:
		Scene();
		virtual ~Scene();

		SceneObject *createSceneObject();
        void addSceneObject(SceneObject *object);

		SceneObject *getSceneObject(int index) const { return m_sceneObjects[index]; }
		int getSceneObjectCount() const { return (int)m_sceneObjects.size(); }

	protected:
		std::vector<SceneObject *> m_sceneObjects;
	};

} /* namespace manta */

#endif /* MANTARAY_SCENE_H */
