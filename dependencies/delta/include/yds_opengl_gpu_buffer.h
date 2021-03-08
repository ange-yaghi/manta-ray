#ifndef YDS_OPENGL_GPU_BUFFER_H
#define YDS_OPENGL_GPU_BUFFER_H

#include "yds_gpu_buffer.h"

class ysOpenGLGPUBuffer : public ysGPUBuffer {
    friend class ysOpenGLDevice;

public:
    ysOpenGLGPUBuffer();
    virtual ~ysOpenGLGPUBuffer();

protected:
    int GetTarget();

    unsigned int m_bufferHandle;
    unsigned int m_vertexArrayHandle;
};

#endif /* YDS_OPENGL_GPU_BUFFER_H */
