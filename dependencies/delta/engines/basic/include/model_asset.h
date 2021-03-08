#ifndef DELTA_BASIC_MODEL_ASSET_H
#define DELTA_BASIC_MODEL_ASSET_H

#include "delta_core.h"

namespace dbasic {

    class Material;
    class Skeleton;
    class AssetManager;

    class ModelAsset : public ysObject {
        friend AssetManager;

    public:
        ModelAsset();
        ~ModelAsset();

        const char *GetName() const { return m_name; }

        ysGPUBuffer *GetVertexBuffer() { return m_vertexBuffer; }
        ysGPUBuffer *GetIndexBuffer() { return m_indexBuffer; }

        int GetVertexCount() const { return m_vertexCount; }
        int GetFaceCount() const { return m_faceCount; }
        int GetBaseVertex() { return m_baseVertex; }
        int GetBaseIndex() { return m_baseIndex; }
        int GetVertexSize() const { return m_vertexSize; }
        int GetBoneMap(int boneIndex) const { return m_boneMap[boneIndex]; }
        int GetBoneCount() const { return m_boneMap.GetNumObjects(); }

        Material *GetMaterial() { return m_material; }
        void SetMaterial(Material *material) { m_material = material; }

    protected:
        char m_name[64];

        Material *m_material;

        ysGPUBuffer *m_vertexBuffer;
        ysGPUBuffer *m_indexBuffer;

        ysExpandingArray<int, 0> m_boneMap;

        int m_baseVertex;
        int m_baseIndex;

        int m_faceCount;
        int m_vertexCount;
        int m_UVChannelCount;

        int m_vertexSize;

        AssetManager *m_manager;
    };

} /* namespace dbasic */

#endif /* DELTA_BASIC_MODEL_ASSET_H */
