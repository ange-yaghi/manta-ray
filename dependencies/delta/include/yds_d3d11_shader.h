#ifndef YDS_D3D11_SHADER_H
#define YDS_D3D11_SHADER_H

#include "yds_shader.h"

#include <d3d11.h>

class ysD3D11Shader : public ysShader {
    friend class ysD3D11Device;

public:
    ysD3D11Shader();
    virtual ~ysD3D11Shader();

    virtual void SetDebugName(const std::string &debugName);

protected:
    ID3D10Blob *m_shaderBlob;

    union {
        ID3D11VertexShader *    m_vertexShader;
        ID3D11PixelShader *        m_pixelShader;
        ID3D11GeometryShader *    m_geometryShader;
    };
};

#endif /* YDS_D3D11_SHADER_H */
