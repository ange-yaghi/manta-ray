#ifndef YDS_OPENGL_RENDER_TARGET_H
#define YDS_OPENGL_RENDER_TARGET_H

#include "yds_render_target.h"

class ysOpenGLRenderTarget : public ysRenderTarget {
    friend class ysOpenGLDevice;

public:
    ysOpenGLRenderTarget();
    virtual ~ysOpenGLRenderTarget();

    unsigned int GetFramebuffer() const { return m_framebufferHandle; }
    unsigned int GetTexture() const { return m_textureHandle; }
    unsigned int GetDepthBuffer() const { return m_depthBufferHandle; }

protected:
    unsigned int m_framebufferHandle;
    unsigned int m_depthBufferHandle;
    unsigned int m_textureHandle;
};

#endif /* YDS_OPENGL_RENDER_TARGET_H */
