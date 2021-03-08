#ifndef YDS_D3D10_SHADER_PROGRAM_H
#define YDS_D3D10_SHADER_PROGRAM_H

#include "yds_shader_program.h"

class ysD3D10Shader;
class ysD3D10ShaderProgram : public ysShaderProgram {
    friend class ysD3D10Device;

public:
    ysD3D10ShaderProgram();
    virtual ~ysD3D10ShaderProgram();

protected:
    ysD3D10Shader *GetShader(ysShader::ShaderType type);
};

#endif /* YDS_D3D10_SHADER_PROGRAM_H */
