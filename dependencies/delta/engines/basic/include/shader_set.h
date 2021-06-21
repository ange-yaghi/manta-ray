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

        template <typename T_ShaderStage=ShaderStage>
        ysError NewStage(const std::string &name, T_ShaderStage **newStage) {
            YDS_ERROR_DECLARE("NewStage");

            T_ShaderStage *stage = new T_ShaderStage();
            m_stages.New() = stage;
            stage->Initialize(m_device, name);

            *newStage = stage;

            return YDS_ERROR_RETURN(ysError::None);
        }

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
