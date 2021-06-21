#ifndef YDS_OPENGL_SHADER_H
#define YDS_OPENGL_SHADER_H

#include "yds_shader.h"

class ysOpenGLShader : public ysShader {
    friend class ysOpenGLDevice;

public:
    ysOpenGLShader();
    virtual ~ysOpenGLShader();

protected:
    unsigned int m_handle;
};

#endif /* YDS_OPENGL_SHADER_H */
