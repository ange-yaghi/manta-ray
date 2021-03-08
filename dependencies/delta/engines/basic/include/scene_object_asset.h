#ifndef DELTA_BASIC_SCENE_OBJECT_ASSET_H
#define DELTA_BASIC_SCENE_OBJECT_ASSET_H

#include "delta_core.h"

namespace dbasic {

    class ModelAsset;
    class Material;
    class AssetManager;

    class SceneObjectAsset : public ysObject {
        friend AssetManager;

    public:
        SceneObjectAsset();
        ~SceneObjectAsset();

        ysMatrix GetLocalTransform() const { return m_localTransform; }
        ysMatrix GetWorldTransform() const;

        void ApplyTransformation(const ysMatrix &transform);

        ysVector GetPosition() const { return m_localPosition; }
        ysVector GetWorldPosition() const;

        ysQuaternion GetLocalOrientation() const { return m_localOrientation; }

        int GetParent() const { return m_parent; }

        ysObjectData::ObjectType GetType() const { return m_type; }

        void AddChild(int index) { m_children.New() = index; }
        int GetChild(int index) { return m_children[index]; }
        int *GetChildren() { return m_children.GetBuffer(); }
        int GetChildrenCount() const { return m_children.GetNumObjects(); }
        void ClearChildren() { m_children.Clear(); }

        void SetName(const char *name) { strcpy_s(m_name, 64, name); }
        const char *GetName() const { return m_name; }

    protected:
        char m_name[64];
        int m_parent;
        int m_skeletonIndex;
        ysObjectData::ObjectType m_type;

        Material *m_material;
        ModelAsset *m_geometry;
        AssetManager *m_manager;

        ysVector m_localPosition;
        ysMatrix m_localTransform;
        ysQuaternion m_localOrientation;

        ysExpandingArray<int, 4> m_children;
    };

} /* namespace dbasic */

#endif /* DELTA_BASIC_SCENE_OBJECT_ASSET_H */
