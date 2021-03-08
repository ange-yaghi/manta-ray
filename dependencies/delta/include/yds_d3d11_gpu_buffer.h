#ifndef YDS_D3D11_GPU_BUFFER_H
#define YDS_D3D11_GPU_BUFFER_H

#include "yds_gpu_buffer.h"
#include <d3d11.h>

class ysD3D11GPUBuffer : public ysGPUBuffer {
    friend class ysD3D11Device;

public:
    ysD3D11GPUBuffer();
    virtual ~ysD3D11GPUBuffer();

    virtual void SetDebugName(const std::string &debugName);

protected:
    ID3D11Buffer *m_buffer;
};

#endif /* YDS_D3D11_GPU_BUFFER_H */
