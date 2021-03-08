#ifndef YDS_SHADER_PROGRAM_H
#define YDS_SHADER_PROGRAM_H

#include "yds_context_object.h"
#include "yds_shader.h"

class ysShaderProgram : public ysContextObject {
    friend class ysDevice;

public:
    ysShaderProgram();
    ysShaderProgram(DeviceAPI API);
    virtual ~ysShaderProgram();

    const ysShader *GetShader(ysShader::ShaderType type) const { return m_shaderSlots[(int)type]; }

protected:
    ysShader *m_shaderSlots[(int)ysShader::ShaderType::NumShaderTypes];
    bool m_isLinked;
};

#endif /* YDS_SHADER_PROGRAM_H */
