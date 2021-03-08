#ifndef DELTA_BASIC_RENDER_NODE_H
#define DELTA_BASIC_RENDER_NODE_H

#include "delta_core.h"

#include "delta_physics.h"

namespace dbasic {

    class RenderSkeleton;
    class ModelAsset;
    class SceneObjectAsset;

    class RenderNode : public ysObject {
    public:
        RenderNode();
        ~RenderNode();

        // Rigid body component
        ysTransform Transform;

        void SetParent(RenderNode *node);
        RenderNode *GetParent();

        const char *GetName() const { return m_name; }
        void SetName(const char *name) { strcpy_s(m_name, 64, name); }

        RenderSkeleton *GetSkeleton() { return m_renderSkeleton; }
        void SetSkeleton(RenderSkeleton *skeleton) { m_renderSkeleton = skeleton; }

        ModelAsset *GetModelAsset() { return m_modelAsset; }
        void SetModelAsset(ModelAsset *modelAsset) { m_modelAsset = modelAsset; }

        void SetAssetID(int assetID) { m_assetID = assetID; }
        int GetAssetID() const { return m_assetID; }

        TransformTarget *GetLocationTarget() { return &m_locationTarget; }
        TransformTarget *GetRotationTarget() { return &m_rotationTarget; }

        void SetBone(bool bone) { m_bone = bone; }
        bool IsBone() const { return m_bone; }

        void SetLastValidOrientation(const ysQuaternion &q) { m_lastValidOrientation = q; }
        ysQuaternion GetLastValidOrientation() const { return m_lastValidOrientation; }

        void SetRestLocation(const ysVector &v) { m_restLocation = v; }
        ysVector GetRestLocation() const { return m_restLocation; }

        void SetRestOrientation(const ysQuaternion &q) { m_restOrientation = q; }
        ysQuaternion GetRestOrientation() const { return m_restOrientation; }

        void SetSceneAsset(SceneObjectAsset *asset) { m_sceneAsset = asset; }
        SceneObjectAsset *GetSceneAsset() const { return m_sceneAsset; }

    protected:
        // Node name
        char m_name[64];

        // Convenient reference to the asset ID this node is derived from
        int m_assetID;

        // Parent node
        RenderNode *m_parent;

        // Parent render skeleton
        RenderSkeleton *m_renderSkeleton;

        // Model asset
        ModelAsset *m_modelAsset;

        // Scene object reference
        SceneObjectAsset *m_sceneAsset;

        bool m_bone;

        // Animation targets
        TransformTarget m_locationTarget;
        TransformTarget m_rotationTarget;

        ysVector m_restLocation;
        ysQuaternion m_restOrientation;
        ysQuaternion m_lastValidOrientation;
    };

} /* namespace dbasic */

#endif /* DELTA_BASIC_RENDER_NODE_H */
