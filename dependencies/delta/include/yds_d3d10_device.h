#ifndef YDS_D3D10_DEVICE_H
#define YDS_D3D10_DEVICE_H

#include "yds_device.h"

#include <D3D10.h>

// --------------------------------------------------------
// Direct3D10 Device
// --------------------------------------------------------

class ysD3D10Device : public ysDevice {
    friend ysDevice;

protected:
    ysD3D10Device();
    virtual ~ysD3D10Device();

public:
    // Setup
    virtual ysError InitializeDevice();
    virtual ysError DestroyDevice();
    virtual bool CheckSupport();

    // State
    virtual ysError SetFaceCulling(bool faceCulling);
    virtual ysError SetFaceCullingMode(CullMode cullMode);

    // Rendering Contexts
    virtual ysError CreateRenderingContext(ysRenderingContext **renderingContext, ysWindow *window);
    virtual ysError UpdateRenderingContext(ysRenderingContext *context);
    virtual ysError DestroyRenderingContext(ysRenderingContext *&context);
    virtual ysError SetContextMode(ysRenderingContext *context, ysRenderingContext::ContextMode mode);

    virtual ysError CreateOnScreenRenderTarget(ysRenderTarget **newTarget, ysRenderingContext *context, bool depthBuffer);
    virtual ysError CreateOffScreenRenderTarget(ysRenderTarget **newTarget, int width, int height, ysRenderTarget::Format format, bool colorData, bool depthBuffer);
    virtual ysError CreateSubRenderTarget(ysRenderTarget **newTarget, ysRenderTarget *parent, int x, int y, int width, int height);
    virtual ysError ResizeRenderTarget(ysRenderTarget *target, int width, int height, int pwidth, int pheight);
    virtual ysError DestroyRenderTarget(ysRenderTarget *&target);
    virtual ysError SetRenderTarget(ysRenderTarget *target, int slot=0);

    virtual ysError ClearBuffers(const float *clearColor);
    virtual ysError Present();

    // GPU Buffers
    virtual ysError CreateVertexBuffer(ysGPUBuffer **newBuffer, int size, char *data, bool mirrorToRam = false);
    virtual ysError CreateIndexBuffer(ysGPUBuffer **newBuffer, int size, char *data, bool mirrorToRam = false);
    virtual ysError CreateConstantBuffer(ysGPUBuffer **newBuffer, int size, char *data, bool mirrorToRam = false);
    virtual ysError UseVertexBuffer(ysGPUBuffer *buffer, int stride, int offset);
    virtual ysError UseIndexBuffer(ysGPUBuffer *buffer, int offset);
    virtual ysError UseConstantBuffer(ysGPUBuffer *buffer, int slot);
    virtual ysError EditBufferDataRange(ysGPUBuffer *buffer, char *data, int size, int offset);
    virtual ysError EditBufferData(ysGPUBuffer *buffer, char *data);
    virtual ysError DestroyGPUBuffer(ysGPUBuffer *&buffer);

    // Shaders
    virtual ysError CreateVertexShader(ysShader **newShader, const char *shaderFilename, const char *shaderName);
    virtual ysError CreatePixelShader(ysShader **newShader, const char *shaderFilename, const char *shaderName);
    virtual ysError DestroyShader(ysShader *&shader);

    // Shader Programs
    virtual ysError CreateShaderProgram(ysShaderProgram **newProgram);
    virtual ysError DestroyShaderProgram(ysShaderProgram *&shader, bool destroyShaders = false);
    virtual ysError AttachShader(ysShaderProgram *targetProgram, ysShader *shader);
    virtual ysError LinkProgram(ysShaderProgram *program);
    virtual ysError UseShaderProgram(ysShaderProgram *);

    // Input Layouts 
    virtual ysError CreateInputLayout(ysInputLayout **newLayout, ysShader *shader, const ysRenderGeometryFormat *format);
    virtual ysError UseInputLayout(ysInputLayout *layout);
    virtual ysError DestroyInputLayout(ysInputLayout *&layout);

    // Textures 
    virtual ysError CreateTexture(ysTexture **texture, const char *fname);
    virtual ysError CreateTexture(ysTexture **texture, int width, int height, const unsigned char *buffer);
    virtual ysError UpdateTexture(ysTexture *texture, const unsigned char *buffer);
    virtual ysError CreateAlphaTexture(ysTexture **texture, int width, int height, const unsigned char *buffer);
    virtual ysError DestroyTexture(ysTexture *&texture);
    virtual ysError UseTexture(ysTexture *texture, int slot);
    virtual ysError UseRenderTargetAsTexture(ysRenderTarget *texture, int slot);

    virtual void Draw(int numFaces, int indexOffset, int vertexOffset);

    // TEMP
    ID3D10RasterizerState *m_rasterizerState;

public:
    // Non-standard interface

    void GetDXGIDevice(IDXGIDevice **device);
    IDXGIFactory *GetDXGIFactory() { return m_DXGIFactory; }
    ID3D10Device *GetDevice() { return m_device; }

    static DXGI_FORMAT ConvertInputLayoutFormat(ysRenderGeometryChannel::ChannelFormat format);

protected:
    ID3D10Device *m_device;
    IDXGIFactory *m_DXGIFactory;

protected:
    // Hidden functionality
    ysError CreateD3D10DepthBuffer(ID3D10DepthStencilView **newDepthStencil, int width, int height, int count, int quality);
    ysError DestroyD3D10DepthBuffer(ID3D10DepthStencilView *&depthStencil);

    ysError CreateD3D10OnScreenRenderTarget(ysRenderTarget *target, ysRenderingContext *context, bool depthBuffer);
    ysError CreateD3D10OffScreenRenderTarget(ysRenderTarget *target, int width, int height, ysRenderTarget::Format format, bool colorData, bool depthBuffer);

    ysError DestroyD3D10RenderTarget(ysRenderTarget *target);
};

#endif /* YDS_D3D10_DEVICE_H */
