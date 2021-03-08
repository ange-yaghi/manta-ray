#ifndef YDS_D3D10_GPU_BUFFER_H
#define YDS_D3D10_GPU_BUFFER_H

#include "yds_gpu_buffer.h"

#include <D3D10.h>

class ysD3D10GPUBuffer : public ysGPUBuffer
{

    friend class ysD3D10Device;

public:

    ysD3D10GPUBuffer();
    virtual ~ysD3D10GPUBuffer();

protected:

    ID3D10Buffer *m_buffer;

};

#endif