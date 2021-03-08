#ifndef YDS_D3D11_SHADER_PROGRAM_H
#define YDS_D3D11_SHADER_PROGRAM_H

#include "yds_shader_program.h"

class ysD3D11Shader;
class ysD3D11ShaderProgram : public ysShaderProgram {
    friend class ysD3D11Device;

public:
    ysD3D11ShaderProgram();
    ~ysD3D11ShaderProgram();

protected:
    ysD3D11Shader *GetShader(ysShader::ShaderType type);
};

#endif /* YDS_D3D11_SHADER_PROGRAM_H */
