#ifndef YDS_D3D10_TEXTURE_H
#define YDS_D3D10_TEXTURE_H

#include "yds_texture.h"

#include <D3D10.h>

class ysD3D10Texture : public ysTexture
{

    friend class ysD3D10Device;

public:

    ysD3D10Texture();
    virtual ~ysD3D10Texture();

protected:

    ID3D10ShaderResourceView *m_resourceView;
    ID3D10RenderTargetView *m_renderTargetView;

};

#endif