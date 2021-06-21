#ifndef YDS_D3D11_DEVICE_H
#define YDS_D3D11_DEVICE_H

#include "yds_device.h"

// DirectX forward declarations
struct IDXGIDevice;
struct ID3D11DeviceContext;
struct IDXGIFactory;
struct ID3D11Device;
struct ID3D11RasterizerState;
struct ID3D11DepthStencilView;
struct ID3D11ShaderResourceView;
enum DXGI_FORMAT;

class ysD3D11Device : public ysDevice {
    friend ysDevice;

private:
    ysD3D11Device();
    virtual ~ysD3D11Device();

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
    virtual ysError SetDepthTestEnabled(ysRenderTarget *target, bool enable);

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
    virtual ysError CreateAlphaTexture(ysTexture **texture, int width, int height, const unsigned char *buffer);
    virtual ysError UpdateTexture(ysTexture *texture, const unsigned char *buffer);
    virtual ysError DestroyTexture(ysTexture *&texture);
    virtual ysError UseTexture(ysTexture *texture, int slot);
    virtual ysError UseRenderTargetAsTexture(ysRenderTarget *renderTarget, int slot);

    virtual void Draw(int numFaces, int indexOffset, int vertexOffset);

public:
    // Non-standard interface
    void GetDXGIDevice(IDXGIDevice **device);
    ID3D11DeviceContext *GetImmediateContext() { return m_deviceContext; }
    IDXGIFactory *GetDXGIFactory() { return m_DXGIFactory; }
    ID3D11Device *GetDevice() { return m_device; }

    static DXGI_FORMAT ConvertInputLayoutFormat(ysRenderGeometryChannel::ChannelFormat format);

    // TEMP
    struct ID3D11RasterizerState *m_rasterizerState;
    struct ID3D11SamplerState *m_samplerState;

protected:
    ID3D11Device *m_device;
    ID3D11DeviceContext *m_deviceContext;

    struct ID3D11DepthStencilState *m_depthStencilEnabledState;
    struct ID3D11DepthStencilState *m_depthStencilDisabledState;

    struct ID3D11BlendState *m_blendState;

    IDXGIFactory *m_DXGIFactory;

    int m_multisampleCount;
    int m_multisampleQuality;

protected:
    // Platform specific functionality
    ysError CreateD3D11DepthStencilView(ID3D11DepthStencilView **newDepthStencil, ID3D11ShaderResourceView **shaderResourceView, int width, int height, int count, int quality, bool shaderResource);
    ysError DestroyD3D11DepthStencilView(ID3D11DepthStencilView *&depthStencil);

    ysError CreateD3D11OnScreenRenderTarget(ysRenderTarget *target, ysRenderingContext *context, bool depthBuffer);
    ysError CreateD3D11OffScreenRenderTarget(ysRenderTarget *target, int width, int height, ysRenderTarget::Format format, bool colorData, bool depthBuffer);

    ysError DestroyD3D11RenderTarget(ysRenderTarget *target);
};

#endif /* YDS_D3D11_DEVICE_H */
