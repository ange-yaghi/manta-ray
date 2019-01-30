#ifndef SCENE_H
#define SCENE_H

#include <vector>

namespace manta {

	class SceneObject;

	class Scene {
	public:
		Scene();
		~Scene();

		SceneObject *createSceneObject();

		SceneObject *getSceneObject(int index) const { return m_sceneObjects[index]; }
		int getSceneObjectCount() const { return (int)m_sceneObjects.size(); }

	protected:
		std::vector<SceneObject *> m_sceneObjects;
	};

} /* namespace manta */

#endif /* SCENE_H */
