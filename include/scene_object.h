#ifndef MANTARAY_SCENE_OBJECT_H
#define MANTARAY_SCENE_OBJECT_H

#include "object_reference_node.h"

#include <string>

namespace manta {

    class SceneGeometry;
    class Material;
    class Scene;

    class SceneObject : public ObjectReferenceNode<SceneObject> {
    public:
        SceneObject();
        virtual ~SceneObject();

        virtual void _evaluate();
        virtual void registerInputs();

        void setGeometry(SceneGeometry *geometry) { m_geometry = geometry; }
        SceneGeometry *getGeometry() { return m_geometry; }

        void setDefaultMaterial(Material *material) { m_defaultMaterial = material; }
        Material *getDefaultMaterial() { return m_defaultMaterial; }

        void setName(const char *name) { m_name = name; }
        const char *getName() const { return m_name.c_str(); }

    protected:
        piranha::pNodeInput m_geometryInput;
        piranha::pNodeInput m_sceneInput;
        piranha::pNodeInput m_defaultMaterialInput;
        piranha::pNodeInput m_nameInput;

    protected:
        Scene *m_scene;
        SceneGeometry *m_geometry;
        Material *m_defaultMaterial;

        std::string m_name;
    };

} /* namespace manta */

#endif /* MANTARAY_SCENE_OBJECT_H */
