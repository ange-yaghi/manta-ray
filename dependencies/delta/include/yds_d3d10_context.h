#ifndef YDS_D3D10_CONTEXT_H
#define YDS_D3D10_CONTEXT_H

#include "yds_window.h"
#include "yds_rendering_context.h"

#include <D3D10.h>

// --------------------------------------------------------
// Direct3D10 Context
// --------------------------------------------------------

class ysD3D10Context : public ysRenderingContext
{

    friend class ysD3D10Device;

public:

    ysD3D10Context();
    virtual ~ysD3D10Context();

protected:

    IDXGISwapChain *m_swapChain;

};

#endif