#ifndef DELTA_BASIC_DELTA_ENGINE_H
#define DELTA_BASIC_DELTA_ENGINE_H

#include "delta_core.h"

#include "window_handler.h"
#include "shader_controls.h"
#include "animation.h"
#include "model_asset.h"
#include "audio_asset.h"
#include "console.h"
#include "font.h"
#include "default_shaders.h"
#include "shader_base.h"

#include "../../../physics/include/mass_spring_system.h"
#include "../../../physics/include/rigid_body_system.h"

namespace dbasic {

    class RenderSkeleton;

    class DeltaEngine : public ysObject {
    public:
        static const std::string FrameBreakdownFull;
        static const std::string FrameBreakdownRenderScene;

        static const int MaxLayers = 256;

        struct DrawCall {
            StageEnableFlags Flags = 0;
            void *ObjectData;
            int ObjectDataSize;
            ysGPUBuffer *IndexBuffer = nullptr;
            ysGPUBuffer *VertexBuffer = nullptr;
            int VertexSize = 0;
            int BaseIndex = 0;
            int BaseVertex = 0;
            int FaceCount = 0;
            bool DepthTest = true;
        };

        struct GameEngineSettings {
            const char *WindowTitle = "Delta_GameWindow";
            void *Instance = nullptr;
            ysWindow::WindowStyle WindowStyle = ysWindow::WindowStyle::Windowed;
            ysContextObject::DeviceAPI API = ysContextObject::DeviceAPI::DirectX11;
            const char *ShaderDirectory = "../DeltaEngineTullahoma/Shaders/";
            const char *LoggingDirectory = "";
            bool DepthBuffer = true;
            bool FrameLogging = false;
            int WindowWidth = 1920;
            int WindowHeight = 1080;
            int WindowPositionX = 0;
            int WindowPositionY = 0;
        };

        static const GameEngineSettings DefaultSettings;

    public:
        DeltaEngine();
        ~DeltaEngine();

        // Physics Interface
        dphysics::MassSpringSystem PhysicsSystem;
        dphysics::RigidBodySystem RBSystem;

        ysError CreateGameWindow(const GameEngineSettings &settings = DefaultSettings);
        ysError StartFrame();
        ysError EndFrame();
        ysError Destroy();

        ysError InitializeShaderSet(ShaderSet *shaderSet);
        ysError InitializeDefaultShaders(DefaultShaders *shaders, ShaderSet *shaderSet);
        ysError InitializeConsoleShaders(ShaderSet *shaderSet);
        void SetShaderSet(ShaderSet *shaderSet) { m_shaderSet = shaderSet; }
        ShaderSet *GetShaderSet() const { return m_shaderSet; }

        ysError DrawSaq(StageEnableFlags flags);
        ysError DrawImage(StageEnableFlags flags, ysTexture *image, int layer = 0);
        ysError DrawBox(StageEnableFlags flags, int layer = 0);
        ysError DrawAxis(StageEnableFlags flags, int layer = 0);
        ysError DrawModel(StageEnableFlags flags, ModelAsset *model, int layer = 0);
        ysError DrawRenderSkeleton(
            StageEnableFlags flags, RenderSkeleton *skeleton, float scale, ShaderBase *shaders, int layer);
        ysError DrawGeneric(
            StageEnableFlags flags, ysGPUBuffer *indexBuffer, ysGPUBuffer *vertexBuffer, int vertexSize,
            int baseIndex, int baseVertex, int faceCount, bool depthTest = true, int layer = 0);
        ysError LoadTexture(ysTexture **image, const char *fname);
        ysError LoadAnimation(Animation **animation, const char *path, int start, int end);
        ysError LoadFont(Font **font, const char *path, int size = 4096, int fontSize = 64);

        ysError PlayAudio(AudioAsset *audio);

        void SubmitSkeleton(Skeleton *skeleton);

        bool IsOpen() const { return m_gameWindow->IsOpen(); }

        void SetWindowSize(int width, int height);
        void SetConsoleColor(const ysVector &v);

        // Input Device
        bool IsKeyDown(ysKey::Code key);
        bool ProcessKeyDown(ysKey::Code key);
        bool ProcessKeyUp(ysKey::Code key);

        bool ProcessMouseButtonDown(ysMouse::Button button);
        bool ProcessMouseButtonUp(ysMouse::Button button);
        bool IsMouseButtonDown(ysMouse::Button button);
        int GetMouseWheel();
        void GetMousePos(int *x, int *y);
        void GetOsMousePos(int *x, int *y);

        void SetCursorPositionLock(bool lock) { m_cursorPositionLocked = lock; }
        bool GetCursorPositionLock() { return m_cursorPositionLocked; }

        void SetCursorHidden(bool hidden) { m_cursorHidden = hidden; }
        bool GetCursorHidden() const { return m_cursorHidden; }

        void SetConsoleEnabled(bool enabled) { m_consoleEnabled = enabled; }
        bool IsConsoleEnabled() const { return m_consoleEnabled; }

        float GetFrameLength();
        float GetAverageFramerate();

        ysDevice *GetDevice() { return m_device; }

        int GetScreenWidth() const;
        int GetScreenHeight() const;

        Console *GetConsole() { return &m_console; }
        UiRenderer *GetUiRenderer() { return &m_uiRenderer; }

        ysAudioDevice *GetAudioDevice() const { return m_audioDevice; }
        ysBreakdownTimer &GetBreakdownTimer() { return m_breakdownTimer; }

        ysWindowSystem *GetWindowSystem() const { return m_windowSystem; }
        ysWindow *GetGameWindow() const { return m_gameWindow; }

        ysShader *GetSaqPixelShader() const { return m_saqPixelShader; }
        ysShader *GetSaqVertexShader() const { return m_saqVertexShader; }

        ysShaderProgram *GetDefaultShaderProgram() const { return m_shaderProgram; }
        ysShaderProgram *GetConsoleShaderProgram() const { return m_consoleProgram; }

        ysInputLayout *GetSaqInputLayout() const { return m_saqInputLayout; }
        ysInputLayout *GetDefaultInputLayout() const { return m_inputLayout; }
        ysInputLayout *GetConsoleInputLayout() const { return m_consoleInputLayout; }

        const ysRenderGeometryFormat *GetGeometryFormat() const { return &m_standardFormat; }

        ysRenderTarget *GetScreenRenderTarget() const { return m_mainRenderTarget; }

    protected:
        ysDevice *m_device;

        ysWindowSystem *m_windowSystem;
        ysInputSystem *m_inputSystem;

        WindowHandler m_windowHandler;
        ysWindow *m_gameWindow;

        ysRenderingContext *m_renderingContext;
        ysRenderTarget *m_mainRenderTarget;

        ysAudioSystem *m_audioSystem;
        ysAudioDevice *m_audioDevice;

        ysGPUBuffer *m_mainVertexBuffer;
        ysGPUBuffer *m_mainIndexBuffer;

        ysKeyboard *m_mainKeyboard;
        ysMouse *m_mainMouse;

        // Shader Controls
        ShaderSet *m_shaderSet;

        ysGPUBuffer *m_consoleShaderObjectVariablesBuffer;
        ConsoleShaderObjectVariables m_consoleShaderObjectVariables;

        ysShader *m_vertexShader;
        ysShader *m_vertexSkinnedShader;
        ysShader *m_pixelShader;
        ysShader *m_consoleVertexShader;
        ysShader *m_consolePixelShader;
        ysShader *m_saqVertexShader;
        ysShader *m_saqPixelShader;

        ysShaderProgram *m_shaderProgram;
        ysShaderProgram *m_skinnedShaderProgram;
        ysShaderProgram *m_consoleProgram;

        ysRenderGeometryFormat m_skinnedFormat;
        ysRenderGeometryFormat m_standardFormat;
        ysRenderGeometryFormat m_consoleVertexFormat;
        ysInputLayout *m_skinnedInputLayout;
        ysInputLayout *m_inputLayout;
        ysInputLayout *m_consoleInputLayout;
        ysInputLayout *m_saqInputLayout;

        // Text Support
        UiRenderer m_uiRenderer;
        Console m_console;

        bool m_initialized;

        // Timing
        ysTimingSystem *m_timingSystem;
        ysBreakdownTimer m_breakdownTimer;

        DrawCall *NewDrawCall(int layer, int objectDataSize);

    protected:
        // Settings
        float m_clearColor[4];
        bool m_consoleEnabled;

        // Cursor
        bool m_cursorPositionLocked;
        bool m_cursorHidden;

    protected:
        // Initialization Routines
        ysError InitializeGeometry();
        ysError InitializeShaders(const char *shaderDirectory);

        ysError InitializeBreakdownTimer(const char *loggingDirectory);

    protected:
        // Drawing queues
        ysExpandingArray<DrawCall, 256> *m_drawQueue;
        ysError ExecuteDrawQueue();
        ysError ExecuteShaderStage(int stageIndex);

        void ClearDrawQueue();
    };

} /* namesapce dbasic */

#endif /* DELTA_BASIC_DELTA_ENGINE_H */
