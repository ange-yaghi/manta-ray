#ifndef YDS_D3D11_RENDER_TARGET_H
#define YDS_D3D11_RENDER_TARGET_H

#include "yds_render_target.h"
#include "yds_depth_buffer.h"

#include <d3d11.h>

class ysD3D11RenderTarget : public ysRenderTarget {
    friend class ysD3D11Device;

public:
    ysD3D11RenderTarget();
    virtual ~ysD3D11RenderTarget();

    virtual void SetDebugName(const std::string &debugName);

protected:
    ID3D11ShaderResourceView *m_resourceView;
    ID3D11RenderTargetView *m_renderTargetView;

    ID3D11DepthStencilView *m_depthStencilView;

    ID3D11DepthStencilState *m_depthTestEnabledState;
    ID3D11DepthStencilState *m_depthTestDisabledState;
};

#endif /* YDS_D3D11_RENDER_TARGET_H */
