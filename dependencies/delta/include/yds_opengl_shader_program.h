#ifndef YDS_OPENGL_SHADER_PROGRAM_H
#define YDS_OPENGL_SHADER_PROGRAM_H

#include "yds_shader_program.h"

class ysOpenGLShader;
class ysOpenGLShaderProgram : public ysShaderProgram {
    friend class ysOpenGLDevice;

public:
    ysOpenGLShaderProgram();
    ~ysOpenGLShaderProgram();

protected:
    ysOpenGLShader *GetShader(ysShader::ShaderType type);

    unsigned int m_handle;
};

#endif /* YDS_OPENGL_SHADER_PROGRAM_H */
