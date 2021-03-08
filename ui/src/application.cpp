#include "../include/application.h"

#include "../../include/os_utilities.h"
#include "../../include/vector_map_2d.h"
#include "../../include/manta_math.h"

#include <Windows.h>

mantaray_ui::Application *mantaray_ui::Application::s_application = nullptr;

mantaray_ui::Application::Application() {
    m_inputFile = "../../workspace/ui_test/script/test.mr";
    m_fileWatcherThread = nullptr;
}

mantaray_ui::Application::~Application() {
    /* void */
}

void mantaray_ui::Application::initialize(void *instance, ysContextObject::DeviceAPI api) {
    dbasic::Path modulePath = dbasic::GetModulePath();
    dbasic::Path confPath = modulePath.Append("delta.conf");

    std::string enginePath = "../../dependencies/delta/engines/basic";
    std::string assetPath = "../../assets";
    if (confPath.Exists()) {
        std::fstream confFile(confPath.ToString(), std::ios::in);
        
        std::getline(confFile, enginePath);
        std::getline(confFile, assetPath);
        enginePath = modulePath.Append(enginePath).ToString();
        assetPath = modulePath.Append(assetPath).ToString();

        confFile.close();
    }

    m_engine.GetConsole()->SetDefaultFontDirectory(enginePath + "/fonts/");

    const std::string shaderPath = enginePath + "/shaders/";

    dbasic::DeltaEngine::GameEngineSettings settings;
    settings.API = api;
    settings.DepthBuffer = true;
    settings.Instance = instance;
    settings.ShaderDirectory = shaderPath.c_str();
    settings.WindowTitle = "Delta Template Application";
    settings.WindowPositionX = 0;
    settings.WindowPositionY = 0;
    settings.WindowStyle = ysWindow::WindowStyle::Windowed;

    m_engine.CreateGameWindow(settings);

    m_engine.InitializeShaderSet(&m_shaderSet);
    m_engine.InitializeDefaultShaders(&m_shaders, &m_shaderSet);
    m_engine.InitializeConsoleShaders(&m_shaderSet);
    m_engine.SetShaderSet(&m_shaderSet);

    m_shaders.SetClearColor(ysColor::srgbiToLinear(0x34, 0x98, 0xdb));

    m_assetManager.SetEngine(&m_engine);

    m_shaders.SetCameraMode(dbasic::DefaultShaders::CameraMode::Flat);

    m_textRenderer.SetEngine(&m_engine);
    m_textRenderer.SetRenderer(m_engine.GetUiRenderer());
    m_textRenderer.SetFont(m_engine.GetConsole()->GetFont());

    m_fileWatcherThread = new std::thread(&Application::listenForFileChanges, this);
    m_fileWatcherThread->detach();

    m_console.initialize(&m_textRenderer);
    manta::Session::get().setConsole(&m_console);

    testTexture = nullptr;
}

ysTexture *mantaray_ui::Application::createTexture(const manta::VectorMap2D *vectorMap) {
    const int width = vectorMap->getWidth();
    const int height = vectorMap->getHeight();
    unsigned char *buffer = new unsigned char[width * height * 4];

    for (int i = 0; i < width; ++i) {
        for (int j = 0; j < height; ++j) {
            const manta::math::Vector v = vectorMap->get(i, j);
            buffer[(j * width + i) * 4 + 0] = (int)(manta::math::getX(v) * 255);
            buffer[(j * width + i) * 4 + 1] = (int)(manta::math::getY(v) * 255);
            buffer[(j * width + i) * 4 + 2] = (int)(manta::math::getZ(v) * 255);
            buffer[(j * width + i) * 4 + 3] = 0xFF;
        }
    }

    ysTexture *newTexture;
    m_engine.GetDevice()->CreateTexture(&newTexture, width, height, buffer);

    return newTexture;
}

void mantaray_ui::Application::recompile() {
    for (CompilerThread *thread : m_compilerThreads) {
        thread->kill();
    }

    CompilerThread *newCompilerThread = new CompilerThread;
    newCompilerThread->initialize();
    newCompilerThread->compileAndExecute(piranha::IrPath(m_inputFile));
    
    m_compilerThreads.push_back(newCompilerThread);
}

void mantaray_ui::Application::process() {
    
}

void mantaray_ui::Application::render() {
    const int screenWidth = m_engine.GetGameWindow()->GetGameWidth();
    const int screenHeight = m_engine.GetGameWindow()->GetGameHeight();

    m_shaders.SetScreenDimensions((float)screenWidth, (float)screenHeight);
    m_shaders.CalculateUiCamera();
    m_shaders.SetFogNear(10000.0f);
    m_shaders.SetFogFar(10001.0f);

    //m_shaders.SetCameraPosition(ysMath::LoadVector(0.0f, 0.0f, 10.0f));
    //m_shaders.SetCameraUp(ysMath::Constants::YAxis);

    if (m_engine.IsKeyDown(ysKey::Code::A)) {
        manta::Session &session = manta::Session::get();
        if (manta::Session::get().getImagePlanePreviewCount() > 0) {
            m_engine.GetDevice()->DestroyTexture(testTexture);

            manta::ImagePlanePreview *preview = manta::Session::get().getImagePlanePreview(manta::Session::get().getImagePlanePreviewCount() - 1);
            if (preview->map != nullptr) {
                testTexture = createTexture(preview->map);
            }
        }
    }

    m_shaders.ResetLights();

    m_console.render();

    BoundingBox bb(screenWidth / 1.5, screenHeight / 1.5);
    bb = bb.AlignLeft(0).AlignBottom(0);
    drawBox(bb, ysColor::srgbiToLinear(255, 255, 255));
}

void mantaray_ui::Application::run() {
    while (m_engine.IsOpen()) {
        m_engine.StartFrame();

        process();
        render();

        m_engine.EndFrame();
    }
}

void mantaray_ui::Application::destroy() {
    m_shaderSet.Destroy();

    m_assetManager.Destroy();
    m_engine.Destroy();
}

void mantaray_ui::Application::drawBox(const BoundingBox &box, const ysVector &color) {
    const int wx = m_engine.GetScreenWidth();
    const int wy = m_engine.GetScreenHeight();

    m_shaders.SetBaseColor(color);
    m_shaders.SetLit(false);
    m_shaders.SetColorReplace(false);
    m_shaders.SetDiffuseTexture(testTexture);
    m_shaders.SetObjectTransform(
        ysMath::TranslationTransform(
            ysMath::LoadVector(
                box.CenterX() - wx / 2.0f,
                box.CenterY() - wy / 2.0f)));
    m_shaders.SetScale(box.Width() / 2, box.Height() / 2);
    m_engine.DrawBox(m_shaders.GetRegularFlags());
}

void mantaray_ui::Application::listenForFileChanges() {
    const manta::Path path(m_inputFile);
    manta::Path parentPath;
    path.getParentPath(&parentPath);

    HANDLE hDir = CreateFile(
        parentPath.toString().c_str(),
        FILE_LIST_DIRECTORY,
        FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
        NULL,
        OPEN_EXISTING,
        FILE_FLAG_BACKUP_SEMANTICS,
        NULL
    );

    wchar_t filename[MAX_PATH];
    FILE_NOTIFY_INFORMATION *buffer = new FILE_NOTIFY_INFORMATION[1024];
    DWORD bytesReturned;

    while (ReadDirectoryChangesW(
        hDir,
        buffer,
        sizeof(FILE_NOTIFY_INFORMATION) * 1024,
        TRUE,
        FILE_NOTIFY_CHANGE_SECURITY | FILE_NOTIFY_CHANGE_CREATION | FILE_NOTIFY_CHANGE_LAST_ACCESS |
        FILE_NOTIFY_CHANGE_LAST_WRITE | FILE_NOTIFY_CHANGE_SIZE | FILE_NOTIFY_CHANGE_ATTRIBUTES |
        FILE_NOTIFY_CHANGE_DIR_NAME | FILE_NOTIFY_CHANGE_FILE_NAME,
        &bytesReturned,
        NULL,
        NULL))
    {
        FILE_NOTIFY_INFORMATION *pNotify;
        pNotify = (FILE_NOTIFY_INFORMATION *)((char *)buffer);
        filename[0] = '\0';

        wcsncpy_s(filename, pNotify->FileName, pNotify->FileNameLength / 2);
        filename[pNotify->FileNameLength / 2] = NULL;

        std::wstring changedFile(filename);

        switch (buffer[0].Action) {
        case FILE_ACTION_MODIFIED:
            recompile();
            break;
        case FILE_ACTION_ADDED:
            break;
        case FILE_ACTION_REMOVED:
            break;
        case FILE_ACTION_RENAMED_OLD_NAME:
            break;
        case FILE_ACTION_RENAMED_NEW_NAME:
            break;
        }
    }

    delete[] buffer;
}
