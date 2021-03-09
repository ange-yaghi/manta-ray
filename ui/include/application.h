#ifndef MANTARAY_UI_APPLICATION_H
#define MANTARAY_UI_APPLICATION_H

#include "../../include/manta.h"
#include "../../include/session.h"

#include "delta.h"
#include "ui_console.h"
#include "compiler_thread.h"
#include "bounding_box.h"

#include <thread>

namespace mantaray_ui {

    class Application {
    protected:
        static Application *s_application;

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

        void drawBox(const BoundingBox &box, const ysVector &color);

        void listenForFileChanges();

        dbasic::TextRenderer *getTextRenderer() { return &m_textRenderer; }

        ysTexture *createTexture(const manta::VectorMap2D *vectorMap);

        void recompile();

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

        std::vector<CompilerThread *> m_compilerThreads;

        ysTexture *testTexture;

        std::atomic<int> m_fileChangeCount;
        double m_fileChangeDebounce;
        bool m_debounceTriggered;
    };

} /* namespace mantaray_ui */

#endif /* MANTARAY_UI_APPLICATION_H */
