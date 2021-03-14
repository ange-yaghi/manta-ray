#ifndef MANTARAY_UI_APPLICATION_H
#define MANTARAY_UI_APPLICATION_H

#include "../../include/manta.h"
#include "../../include/session.h"

#include "delta.h"
#include "ui_console.h"
#include "compiler_thread.h"
#include "bounding_box.h"
#include "preview.h"

#include <thread>

namespace mantaray_ui {

    class Application {
    protected:
        static Application *s_application;

        enum class State {
            Undefined,
            Ready,
            CompilingAndExecuting,
            Killing,
            Finalizing,
            Destroying
        };

    public:
        static const ysVector StandardBlue;
        static const ysVector StandardRed;
        static const ysVector StandardYellow;

    public:
        Application();
        ~Application();

        void initialize(void *instance, ysContextObject::DeviceAPI api);
        void run();
        void destroy();

        void drawImage(ysTexture *texture, const BoundingBox &box, const ysVector &color, const ysVector &scale, const ysVector &pan);
        void drawBox(const BoundingBox &box, const ysVector &color, const ysVector &scale, const ysVector &pan);

        void clearPreviews();

        void listenForFileChanges();

        dbasic::TextRenderer *getTextRenderer() { return &m_textRenderer; }

        static float clamp(float s, float left, float right);
        void compile();

        BoundingBox fitImage(int width, int height, const BoundingBox &extents);

        void fsm();
        void fsmChangeState(State nextState);

        Preview *getPreviewForNode(manta::PreviewNode *node);

    protected:
        void process();
        void render();

        dbasic::ShaderSet m_shaderSet;
        dbasic::DefaultShaders m_shaders;

        dbasic::DeltaEngine m_engine;
        dbasic::AssetManager m_assetManager;

        dbasic::Font *m_font;

        dbasic::TextRenderer m_textRenderer;
        UiConsole m_console;

        std::string m_inputFile;

        std::thread *m_fileWatcherThread;

        CompilerThread * m_compilerThread;

        std::vector<Preview *> m_previews;
        Preview *m_activePreview;
        
        float m_blendLocation;
        int m_updateIndex;

        State m_currentState;

        std::atomic<int> m_fileChangeCount;
        std::atomic<bool> m_textureUpdateComplete;
        double m_fileChangeDebounce;
        bool m_debounceTriggered;

        double m_updateTimer;

        int m_lastMouseWheel;
        
        bool m_dragging;
        int m_lastMouseX;
        int m_lastMouseY;

        std::atomic<bool> m_compileTriggered;
        std::atomic<bool> m_manualCompileTriggered;
        std::atomic<bool> m_cancelTriggered;
    };

} /* namespace mantaray_ui */

#endif /* MANTARAY_UI_APPLICATION_H */
