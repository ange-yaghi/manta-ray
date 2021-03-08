#ifndef YDS_OPENGL_CONTEXT_H
#define YDS_OPENGL_CONTEXT_H

#include "yds_window.h"
#include "yds_rendering_context.h"

#include <OpenGL.h>

class ysOpenGLVirtualContext : public ysRenderingContext {
    friend class ysOpenGLDevice;

public:
    ysOpenGLVirtualContext();
    ysOpenGLVirtualContext(ysWindowSystemObject::Platform platform);
    virtual ~ysOpenGLVirtualContext();

    virtual ysError DestroyContext() {
        return YDS_ERROR_RETURN(ysError::None); 
    }

    virtual ysError TransferContext(ysOpenGLVirtualContext *context) {
        (void)context; 
        return YDS_ERROR_RETURN(ysError::None); 
    }

    virtual ysError SetContextMode(ContextMode mode) {
        (void)mode; 
        return YDS_ERROR_RETURN(ysError::None); 
    }

    virtual ysError SetContext(ysRenderingContext *realContext) {
        (void)realContext; 
        return YDS_ERROR_RETURN(ysError::None); 
    }

    virtual ysError Present() {
        return YDS_ERROR_RETURN(ysError::None); 
    }

    // OpenGL Extensions
    PFNGLGENBUFFERSPROC glGenBuffers = nullptr;
    PFNGLDELETEBUFFERSPROC glDeleteBuffers = nullptr;
    PFNGLDELETEVERTEXARRAYSPROC glDeleteVertexArrays = nullptr;
    PFNGLBINDBUFFERPROC glBindBuffer = nullptr;
    PFNGLBINDBUFFERRANGEPROC glBindBufferRange = nullptr;
    PFNGLBUFFERDATAPROC glBufferData = nullptr;
    PFNGLGENVERTEXARRAYSPROC glGenVertexArrays = nullptr;
    PFNGLBINDVERTEXARRAYPROC glBindVertexArray = nullptr;
    PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray = nullptr;
    PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer = nullptr;
    PFNGLVERTEXATTRIBIPOINTERPROC glVertexAttribIPointer = nullptr;

    PFNGLVERTEXATTRIB3FPROC glVertexAttrib3f = nullptr;
    PFNGLVERTEXATTRIB4FPROC glVertexAttrib4f = nullptr;

    PFNGLDELETEPROGRAMPROC glDeleteProgram = nullptr;
    PFNGLDELETESHADERPROC glDeleteShader = nullptr;

    PFNGLCREATESHADERPROC glCreateShader = nullptr;
    PFNGLSHADERSOURCEPROC glShaderSource = nullptr;
    PFNGLCOMPILESHADERPROC glCompileShader = nullptr;
    PFNGLCREATEPROGRAMPROC glCreateProgram = nullptr;
    PFNGLATTACHSHADERPROC glAttachShader = nullptr;
    PFNGLDETACHSHADERPROC glDetachShader = nullptr;
    PFNGLLINKPROGRAMPROC glLinkProgram = nullptr;
    PFNGLUSEPROGRAMPROC glUseProgram = nullptr;
    PFNGLBINDATTRIBLOCATIONPROC glBindAttribLocation = nullptr;
    PFNGLBINDFRAGDATALOCATIONPROC glBindFragDataLocation = nullptr;
    PFNGLGETFRAGDATALOCATIONPROC glGetFragDataLocation = nullptr;
    PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation = nullptr;
    PFNGLGETSHADERIVPROC glGetShaderiv = nullptr;
    PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog = nullptr;
    PFNGLDRAWBUFFERSPROC glDrawBuffers = nullptr;

    PFNGLUNIFORM4FPROC glUniform4f = nullptr;
    PFNGLUNIFORM4FVPROC glUniform4fv = nullptr;
    PFNGLUNIFORM3FVPROC glUniform3fv = nullptr;
    PFNGLUNIFORM2FVPROC glUniform2fv = nullptr;
    PFNGLUNIFORM3FPROC glUniform3f = nullptr;
    PFNGLUNIFORM2FPROC glUniform2f = nullptr;
    PFNGLUNIFORM1FPROC glUniform1f = nullptr;
    PFNGLUNIFORM1IPROC glUniform1i = nullptr;

    PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv = nullptr;
    PFNGLUNIFORMMATRIX3FVPROC glUniformMatrix3fv = nullptr;

    PFNGLGETPROGRAMIVPROC glGetProgramiv = nullptr;
    PFNGLGETACTIVEUNIFORMNAMEPROC glGetActiveUniformName = nullptr;
    PFNGLGETACTIVEUNIFORMSIVPROC glGetActiveUniformsiv = nullptr;
    PFNGLGETACTIVEUNIFORMPROC glGetActiveUniform = nullptr;

    PFNGLDRAWELEMENTSBASEVERTEXPROC glDrawElementsBaseVertex = nullptr;

    // Textures

    PFNGLTEXIMAGE2DMULTISAMPLEPROC glTexImage2DMultisample = nullptr;
    PFNGLACTIVETEXTUREPROC glActiveTexture = nullptr;
    PFNGLGENERATEMIPMAPPROC glGenerateMipmap = nullptr;

    // Buffers

    PFNGLMAPBUFFERPROC glMapBuffer = nullptr;
    PFNGLMAPBUFFERRANGEPROC glMapBufferRange = nullptr;
    PFNGLUNMAPBUFFERPROC glUnmapBuffer = nullptr;

    PFNGLGENRENDERBUFFERSPROC glGenRenderbuffers = nullptr;
    PFNGLDELETERENDERBUFFERSPROC glDeleteRenderbuffers = nullptr;
    PFNGLBINDRENDERBUFFERPROC glBindRenderbuffer = nullptr;
    PFNGLRENDERBUFFERSTORAGEPROC glRenderbufferStorage = nullptr;
    PFNGLRENDERBUFFERSTORAGEMULTISAMPLEPROC glRenderbufferStorageMultisample = nullptr;

    PFNGLCOPYBUFFERSUBDATAPROC glCopyBufferSubData = nullptr;
    PFNGLBUFFERSUBDATAPROC glBufferSubData = nullptr;

    PFNGLGENFRAMEBUFFERSPROC glGenFramebuffers = nullptr;
    PFNGLDELETEFRAMEBUFFERSPROC glDeleteFramebuffers = nullptr;
    PFNGLBINDFRAMEBUFFERPROC glBindFramebuffer = nullptr;
    PFNGLFRAMEBUFFERTEXTURE2DPROC glFramebufferTexture2D = nullptr;
    PFNGLFRAMEBUFFERRENDERBUFFERPROC glFramebufferRenderbuffer = nullptr;
    PFNGLCHECKFRAMEBUFFERSTATUSPROC glCheckFramebufferStatus = nullptr;

    PFNGLBLITFRAMEBUFFERPROC glBlitFramebuffer = nullptr;
    PFNGLBLENDEQUATIONPROC glBlendEquation = nullptr;

    PFNWGLMAKECONTEXTCURRENTARBPROC wglMakeContextCurrent = nullptr;
    PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = nullptr;
    PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB = nullptr;

    bool IsRealContext() { return m_isRealContext; }

protected:
    bool m_isRealContext;
};

#endif /* YDS_OPENGL_CONTEXT_H */
