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

        struct PreviewSnapshot {
            ysTexture *texture;
            int index;
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

        void listenForFileChanges();

        dbasic::TextRenderer *getTextRenderer() { return &m_textRenderer; }

        ysTexture *createTexture(const manta::VectorMap2D *vectorMap);

        void updateImageThread(const manta::VectorMap2D *vectorMap, int index);
        static float clamp(float s, float left, float right);
        void getBlend(ysTexture **texture0, ysTexture **texture1, float &blend, float &extent);
        void updateImage();
        void recompile();

        BoundingBox fitImage(int width, int height, const BoundingBox &extents);

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

        std::mutex m_textureLock;
        std::vector<PreviewSnapshot> m_previewSnapshots;
        float m_blendLocation;
        int m_updateIndex;

        std::atomic<int> m_fileChangeCount;
        std::atomic<bool> m_textureUpdateComplete;
        double m_fileChangeDebounce;
        bool m_debounceTriggered;

        double m_updateTimer;

        int m_lastMouseWheel;
        int m_zoom;
        bool m_dragging;
        int m_lastMouseX;
        int m_lastMouseY;

        ysVector m_pan;
    };

} /* namespace mantaray_ui */

#endif /* MANTARAY_UI_APPLICATION_H */
