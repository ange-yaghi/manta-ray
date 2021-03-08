#ifndef YDS_SHADER_H
#define YDS_SHADER_H

#include "yds_context_object.h"

class ysShader : public ysContextObject {
    friend class ysDevice;

public:
    static const int MaxShaderFilenameLength = 256;
    static const int MaxShaderNameLength = 64;

    enum class ShaderType {
        Pixel,
        Vertex,

        NumShaderTypes
    };

public:
    ysShader();
    ysShader(DeviceAPI API);
    virtual ~ysShader();

    const char *GetFilename() const { return m_filename; }
    const char *GetShaderName() const { return m_shaderName; }

    ShaderType GetShaderType() const { return m_shaderType; }

protected:
    char m_filename[MaxShaderFilenameLength];
    char m_shaderName[MaxShaderNameLength];

    ShaderType m_shaderType;
};

#endif /* YDS_SHADER_H */
