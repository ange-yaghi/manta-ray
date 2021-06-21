#ifndef YDS_DEVICE_H
#define YDS_DEVICE_H

#include "yds_base.h"
#include "yds_window.h"
#include "yds_context_object.h"
#include "yds_rendering_context.h"
#include "yds_gpu_buffer.h"
#include "yds_shader.h"
#include "yds_input_layout.h"
#include "yds_shader_program.h"
#include "yds_texture.h"

struct ysTextureSlot {
    ysRenderTarget *RenderTarget;
    ysTexture *Texture;
};

class ysDevice : public ysContextObject {
protected:
    ysDevice();
    ysDevice(ysContextObject::DeviceAPI API);
    virtual ~ysDevice();

    static constexpr int MaxRenderTargets = 2;

public:
    enum class CullMode {
        Front,
        Back,
        None
    };

public:
    static ysError CreateDevice(ysDevice **device, DeviceAPI API);

    /* Main Device Interface */

    // Initialize graphics device
    virtual ysError InitializeDevice() = 0;

    // Destroy graphics device
    virtual ysError DestroyDevice() = 0;

    // Check support for this device
    virtual bool CheckSupport() = 0;


    /* Rendering Contexts */

    // Create a new rendering context
    virtual ysError CreateRenderingContext(ysRenderingContext **renderingContext, ysWindow *window) = 0;

    // Update a rendering context
    virtual ysError UpdateRenderingContext(ysRenderingContext *context) = 0;

    // Destroy rendering context
    virtual ysError DestroyRenderingContext(ysRenderingContext *&context);

    // Set the mode of a rendering context
    virtual ysError SetContextMode(ysRenderingContext *context, ysRenderingContext::ContextMode mode);

    // Get the number of created rendering contexts
    int GetRenderingContextCount() { return m_renderingContexts.GetNumObjects(); }


    /* State */

    // Enable/disable face culling
    virtual ysError SetFaceCulling(bool faceCulling) = 0;

    // Set face culling mode
    virtual ysError SetFaceCullingMode(CullMode cullMode) = 0;


    /* Render Targets */

    // Create an on-screen render target
    virtual ysError CreateOnScreenRenderTarget(ysRenderTarget **newTarget, ysRenderingContext *context, bool depthBuffer) = 0;

    // Create an off-screen render target
    virtual ysError CreateOffScreenRenderTarget(ysRenderTarget **newTarget, int width, int height, ysRenderTarget::Format format, bool colorData = true, bool depthBuffer = true) = 0;

    // Create a off-screen copy
    virtual ysError CreateOffScreenRenderTarget(ysRenderTarget **newTarget, const ysRenderTarget *reference);

    // Create a sub render target
    virtual ysError CreateSubRenderTarget(ysRenderTarget **newTarget, ysRenderTarget *parent, int x, int y, int width, int height) = 0;

    // Resize a render target
    virtual ysError ResizeRenderTarget(ysRenderTarget *target, int width, int height, int pwidth, int pheight);

    // Enable/disable depth testing
    virtual ysError SetDepthTestEnabled(ysRenderTarget *target, bool enable);

    // Destroy a render target
    virtual ysError DestroyRenderTarget(ysRenderTarget *&target);

    // Set the active rendering target
    virtual ysError SetRenderTarget(ysRenderTarget *target, int slot=0);

    /* Scene start/end */

    // Clear the current render target
    virtual ysError ClearBuffers(const float *clearColor) = 0;

    // Present the current on-screen render target
    virtual ysError Present() = 0;


    /* Vertex Buffers */

    // Create vertex buffer
    virtual ysError CreateVertexBuffer(ysGPUBuffer **newBuffer, int size, char *data, bool mirrorToRam = false) = 0;

    // Create index buffer
    virtual ysError CreateIndexBuffer(ysGPUBuffer **newBuffer, int size, char *data, bool mirrorToRam = false) = 0;

    // Create constant buffer
    virtual ysError CreateConstantBuffer(ysGPUBuffer **newBuffer, int size, char *data, bool mirrorToRam = false) = 0;

    // Enable a vertex buffer
    virtual ysError UseVertexBuffer(ysGPUBuffer *buffer, int stride, int offset);

    // Enable an index buffer
    virtual ysError UseIndexBuffer(ysGPUBuffer *buffer, int offset);

    // Enable a constant buffer
    virtual ysError UseConstantBuffer(ysGPUBuffer *buffer, int slot);

    // Get the active buffer in any slot
    ysGPUBuffer *GetActiveBuffer(ysGPUBuffer::GPU_BUFFER_TYPE bufferType);

    // Edit the data in a section of a buffer
    virtual ysError EditBufferDataRange(ysGPUBuffer *buffer, char *data, int size, int offset);

    // Replace all data in a buffer
    virtual ysError EditBufferData(ysGPUBuffer *buffer, char *data);

    // Delete a GPU buffer
    virtual ysError DestroyGPUBuffer(ysGPUBuffer *&buffer);


    /* Shaders */

    // Create a vertex shader from a file
    virtual ysError CreateVertexShader(ysShader **newShader, const char *shaderFilename, const char *shaderName) = 0;

    // Create a pixel shader from a file
    virtual ysError CreatePixelShader(ysShader **newShader, const char *shaderFilename, const char *shaderName) = 0;

    // Destroy a shader
    virtual ysError DestroyShader(ysShader *&shader);


    /* Shader Programs */

    // Create a shader program
    virtual ysError CreateShaderProgram(ysShaderProgram **newProgram) = 0;

    // Destroy a shader program
    virtual ysError DestroyShaderProgram(ysShaderProgram *&shader, bool destroyShaders = false);

    // Attach a shader to a shader program
    virtual ysError AttachShader(ysShaderProgram *targetProgram, ysShader *shader);

    // Link a program
    virtual ysError LinkProgram(ysShaderProgram *program);

    // Enable a shader program
    virtual ysError UseShaderProgram(ysShaderProgram *);


    /* Input Layouts */

    // Create an input layout for a shader and format
    virtual ysError CreateInputLayout(ysInputLayout **newLayout, ysShader *shader, const ysRenderGeometryFormat *format) = 0;

    // Enable an input layout
    virtual ysError UseInputLayout(ysInputLayout *layout);

    // Destroy an input layout
    virtual ysError DestroyInputLayout(ysInputLayout *&layout);


    /* Textures */

    // Create a texture from a file
    virtual ysError CreateTexture(ysTexture **texture, const char *fname) = 0;

    // Create an rgb texture from an in-memory buffer
    virtual ysError CreateTexture(ysTexture **texture, int width, int height, const unsigned char *buffer) = 0;

    // Create an alpha texture from an in-memory buffer
    virtual ysError CreateAlphaTexture(ysTexture **texture, int width, int height, const unsigned char *buffer) = 0;

    // Update a texture
    virtual ysError UpdateTexture(ysTexture *texture, const unsigned char *buffer) = 0;

    // Destroy a texture
    virtual ysError DestroyTexture(ysTexture *&texture);

    // Enable a texture
    virtual ysError UseTexture(ysTexture *texture, int slot);

    // Enable a texture in the form of a render target
    virtual ysError UseRenderTargetAsTexture(ysRenderTarget *texture, int slot);

    // Initialize texture slots
    ysError InitializeTextureSlots(int maxSlots);

    /* Debug */

    // TEMP
    virtual void Draw(int numFaces, int indexOffset, int vertexOffset) { (void)numFaces; (void)indexOffset; (void)vertexOffset; }

    ysRenderTarget *GetActiveRenderTarget(int slot=0) const { return m_activeRenderTarget[slot]; }

    void SetDebugFlag(int flag, bool state);
    bool GetDebugFlag(int flag) const;

protected:
    ysRenderTarget *GetActualRenderTarget(int slot);

protected:
    // Object Holders
    ysDynamicArray<ysRenderingContext, 4>    m_renderingContexts;
    ysDynamicArray<ysRenderTarget, 4>        m_renderTargets;
    ysDynamicArray<ysGPUBuffer, 16>            m_gpuBuffers;
    ysDynamicArray<ysShader, 16>            m_shaders;
    ysDynamicArray<ysShaderProgram, 8>        m_shaderPrograms;
    ysDynamicArray<ysInputLayout, 16>        m_inputLayouts;
    ysDynamicArray<ysTexture, 32>            m_textures;

    // Active Objects
    ysRenderTarget *m_activeRenderTarget[MaxRenderTargets];
    ysRenderingContext *m_activeContext;

    ysGPUBuffer *m_activeVertexBuffer;
    ysGPUBuffer *m_activeIndexBuffer;
    ysGPUBuffer *m_activeConstantBuffer;

    ysShaderProgram *m_activeShaderProgram;

    ysInputLayout *m_activeInputLayout;

    ysTextureSlot *m_activeTextures;

    // Debug
    unsigned int m_debugFlags;

    // Platform Dependant Constants
    int m_maxTextureSlots;
};

#endif /* YDS_DEVICE_H */
