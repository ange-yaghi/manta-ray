#ifndef MANTARAY_SCENE_H
#define MANTARAY_SCENE_H

#include "object_reference_node.h"

#include <vector>

namespace manta {

    class SceneObject;
    class Light;

    class Scene : public ObjectReferenceNode<Scene> {
    public:
        Scene();
        virtual ~Scene();

        SceneObject *createSceneObject();
        void addSceneObject(SceneObject *object);

        SceneObject *getSceneObject(int index) const { return m_sceneObjects[index]; }
        int getSceneObjectCount() const { return (int)m_sceneObjects.size(); }

        void addLight(Light *light) { m_lights.push_back(light); }
        Light *getLight(int index) const { return m_lights[index]; }
        int getLightCount() const { return (int)m_lights.size(); }

    protected:
        virtual void _evaluate();

        std::vector<SceneObject *> m_sceneObjects;
        std::vector<Light *> m_lights;
    };

    class SceneAddLight : public ObjectReferenceNode<Scene> {
    public:
        SceneAddLight();
        virtual ~SceneAddLight();

    protected:
        virtual void _evaluate();
        virtual void registerInputs();

        piranha::pNodeInput m_sceneInput;
        piranha::pNodeInput m_lightInput;
    };

} /* namespace manta */

#endif /* MANTARAY_SCENE_H */
