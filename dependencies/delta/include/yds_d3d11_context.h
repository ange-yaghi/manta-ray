#ifndef YDS_D3D11_CONTEXT_H
#define YDS_D3D11_CONTEXT_H

#include "yds_window.h"
#include "yds_rendering_context.h"

struct IDXGISwapChain;

class ysD3D11Context : public ysRenderingContext {
    friend class ysD3D11Device;

public:
    ysD3D11Context();
    virtual ~ysD3D11Context();

protected:
    IDXGISwapChain *m_swapChain;
};

#endif /* YDS_D3D11_CONTEXT_H */
