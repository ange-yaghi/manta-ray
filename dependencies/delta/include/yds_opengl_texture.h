#ifndef YDS_OPENGL_TEXTURE_H
#define YDS_OPENGL_TEXTURE_H

#include "yds_texture.h"

class ysOpenGLTexture : public ysTexture {
    friend class ysOpenGLDevice;

public:
    ysOpenGLTexture();
    ~ysOpenGLTexture();

    unsigned int m_handle;
};

#endif /* YDS_OPENGL_TEXTURE_H */
