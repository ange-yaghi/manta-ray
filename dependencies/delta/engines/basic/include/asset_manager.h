#ifndef DELTA_BASIC_ASSET_MANAGER_H
#define DELTA_BASIC_ASSET_MANAGER_H

#include "delta_core.h"

#include "material.h"
#include "model_asset.h"
#include "scene_object_asset.h"
#include "skeleton.h"
#include "render_skeleton.h"
#include "animation_export_data.h"
#include "animation_object_controller.h"
#include "texture_asset.h"
#include "audio_asset.h"

#include <vector>

namespace dbasic {

    struct CompiledHeader {
        int ObjectCount;
    };

    class DeltaEngine;

    class AssetManager : public ysObject {
    public:
        AssetManager();
        ~AssetManager();

        ysError Destroy();

        ysError CompileSceneFile(const char *fname, float scale = 1.0f, bool force = false);
        ysError CompileInterchangeFile(const char *fname, float scale = 1.0f, bool force = false);
        ysError LoadSceneFile(const char *fname, bool placeInVram = true);

        ysError LoadAnimationFile(const char *fname);
        ysAnimationAction *GetAction(const char *name);
        int GetActionCount() const { return m_actions.GetNumObjects(); }

        ysError LoadTexture(const char *fname, const char *name);
        TextureAsset *GetTexture(const char *name);
        int GetTextureCount() const { return m_textures.GetNumObjects(); }

        ysError LoadAudioFile(const char *fname, const char *name);
        AudioAsset *GetAudioAsset(const char *name);
        int GetAudioAssetCount() const { return m_audioAssets.GetNumObjects(); }

        ysError CompileAnimationFileLegacy(const char *fname);
        ysError LoadAnimationFileLegacy(const char *fname);

        Material *NewMaterial();
        Material *FindMaterial(const char *name);
        Material *GetMaterial(int index) { return m_materials.Get(index); }
        int GetMaterialCount() const { return m_materials.GetNumObjects(); }

        SceneObjectAsset *NewSceneObject();
        int GetSceneObjectCount() const { return m_sceneObjects.GetNumObjects(); }
        SceneObjectAsset *GetSceneObject(int index) { return (index < 0) ? nullptr : m_sceneObjects.Get(index); }
        SceneObjectAsset *GetSceneObject(const char *fname, ysObjectData::ObjectType type);
        SceneObjectAsset *GetRoot(SceneObjectAsset *object);

        ModelAsset *NewModelAsset();
        ModelAsset *GetModelAsset(int index) { return (index < 0) ? nullptr : m_modelAssets.Get(index); }
        ModelAsset *GetModelAsset(const char *name);

        Skeleton *BuildSkeleton(ModelAsset *model);

        RenderSkeleton *BuildRenderSkeleton(ysTransform *root, SceneObjectAsset *rootBone);
        void ProcessRenderNode(SceneObjectAsset *asset, RenderSkeleton *skeleton, RenderNode *parent, RenderNode *top);

        AnimationObjectController *BuildAnimationObjectController(const char *name, ysTransform *transform);

        void SetEngine(DeltaEngine *engine) { m_engine = engine; }
        DeltaEngine *GetEngine() const { return m_engine; }

        ysError ResolveNodeHierarchy();

    protected:
        ysDynamicArray<ModelAsset, 4> m_modelAssets;
        ysDynamicArray<SceneObjectAsset, 4> m_sceneObjects;
        ysDynamicArray<Material, 4> m_materials;
        ysDynamicArray<Skeleton, 4> m_skeletons;
        ysDynamicArray<RenderSkeleton, 4> m_renderSkeletons;
        ysDynamicArray<AnimationObjectController, 4> m_animationControllers;
        ysDynamicArray<AnimationExportData, 4> m_animationExportData;
        ysDynamicArray<ysAnimationAction, 4> m_actions;
        ysDynamicArray<TextureAsset, 4> m_textures;
        ysDynamicArray<AudioAsset, 4> m_audioAssets;

        DeltaEngine *m_engine;

        std::vector<ysGPUBuffer *> m_buffers;
    };

} /* namespace dbasic */

#endif /* DELTA_BASIC_ASSET_MANAGER_H */
