#ifndef DELTA_BASIC_SHADER_SET_H
#define DELTA_BASIC_SHADER_SET_H

#include "delta_core.h"

#include "shader_stage.h"

namespace dbasic {

    class ShaderSet : public ysObject {
    public:
        ShaderSet();
        virtual ~ShaderSet();

        virtual ysError Initialize(ysDevice *device);
        ysError Destroy();

        ysError NewStage(const std::string &name, ShaderStage **newStage);

        int GetObjectDataSize() const;

        ysError CacheObjectData(void *memory, int size);
        ysError ReadObjectData(const void *memory, int size);
        ysError ReadObjectData(const void *memory, int stageIndex, int size);
        void Reset();

        int GetStageCacheOffset(int index) const;
        ShaderStage *GetStage(int index) const { return m_stages[index]; }
        int GetStageCount() const { return m_stages.GetNumObjects(); }

        ysError MergeAndClear(ShaderSet *shaderSet);

    protected:
        ysError ShallowDestroy();

        int GetLargestPossibleIndex(const ShaderStage *stage) const;

    protected:
        ysExpandingArray<ShaderStage *> m_stages;

        ysDevice *m_device;
    };

} /* namespace dbasic */

#endif /* DELTA_BASIC_SHADER_SET_H */
