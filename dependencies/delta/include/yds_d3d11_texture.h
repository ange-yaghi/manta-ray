#ifndef YDS_D3D11_TEXTURE_H
#define YDS_D3D11_TEXTURE_H

#include "yds_texture.h"

#include <D3D11.h>

class ysD3D11Texture : public ysTexture {
    friend class ysD3D11Device;

public:
    ysD3D11Texture();
    ~ysD3D11Texture();

    virtual void SetDebugName(const std::string &debugName);

protected:
    ID3D11ShaderResourceView *m_resourceView;
    ID3D11RenderTargetView *m_renderTargetView;
    ID3D11Texture2D *m_textureResource;
};

#endif /* YDS_D3D11_TEXTURE_H */
