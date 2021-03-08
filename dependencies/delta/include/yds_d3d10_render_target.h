#ifndef YDS_D3D10_RENDER_TARGET_H
#define YDS_D3D10_RENDER_TARGET_H

#include "yds_render_target.h"
#include "yds_depth_buffer.h"

#include <D3D10.h>

class ysD3D10RenderTarget : public ysRenderTarget
{

    friend class ysD3D10Device;

public:

    ysD3D10RenderTarget();
    virtual ~ysD3D10RenderTarget();

protected:

    ID3D10ShaderResourceView *m_resourceView;
    ID3D10RenderTargetView *m_renderTargetView;
    ID3D10DepthStencilView *m_depthStencil;

};

#endif