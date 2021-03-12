#include "../include/application.h"

#include "../../include/os_utilities.h"
#include "../../include/vector_map_2d.h"
#include "../../include/manta_math.h"
#include "../include/grid.h"

#include <Windows.h>

mantaray_ui::Application *mantaray_ui::Application::s_application = nullptr;

const ysVector mantaray_ui::Application::StandardBlue = ysColor::srgbiToSrgb(0x00A4E9);
const ysVector mantaray_ui::Application::StandardRed = ysColor::srgbiToSrgb(0xFF1549);
const ysVector mantaray_ui::Application::StandardYellow = ysColor::srgbiToSrgb(0xFF9F15);

mantaray_ui::Application::Application() {
    m_inputFile = "../../workspace/ui_test/script/test.mr";
    m_fileWatcherThread = nullptr;
    m_fileChangeCount = 0;
    m_fileChangeDebounce = 0.0f;
    m_debounceTriggered = false;
    m_zoom = 0;
    m_updateTimer = 0.0f;

    m_lastMouseX = 0;
    m_lastMouseY = 0;

    m_pan = ysMath::Constants::Zero;
    m_dragging = false;
    m_textureUpdateComplete = true;
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
    m_engine.SetConsoleEnabled(false);

    const std::string shaderPath = enginePath + "/shaders/";

    dbasic::DeltaEngine::GameEngineSettings settings;
    settings.API = api;
    settings.DepthBuffer = true;
    settings.Instance = instance;
    settings.ShaderDirectory = shaderPath.c_str();
    settings.WindowTitle = "MantaRay Preview";
    settings.WindowPositionX = 0;
    settings.WindowPositionY = 0;
    settings.WindowStyle = ysWindow::WindowStyle::Windowed;

    m_engine.CreateGameWindow(settings);

    m_engine.InitializeShaderSet(&m_shaderSet);
    m_engine.InitializeDefaultShaders(&m_shaders, &m_shaderSet);
    m_engine.InitializeConsoleShaders(&m_shaderSet);
    m_engine.SetShaderSet(&m_shaderSet);

    m_shaders.SetClearColor(ysColor::srgbiToLinear(0x0B0D10));

    m_assetManager.SetEngine(&m_engine);

    m_shaders.SetCameraMode(dbasic::DefaultShaders::CameraMode::Flat);
    m_shaderSet.GetStage(0)->GetRenderTarget()->SetDepthTestEnabled(false);

    m_textRenderer.SetEngine(&m_engine);
    m_textRenderer.SetRenderer(m_engine.GetUiRenderer());
    m_textRenderer.SetFont(m_engine.GetConsole()->GetFont());

    m_fileWatcherThread = new std::thread(&Application::listenForFileChanges, this);
    m_fileWatcherThread->detach();

    m_engine.LoadFont(&m_font, "../../assets/fonts/overpass-mono/overpass-mono-regular.otf", 4096, 16);
    m_console.initialize(&m_textRenderer, m_font);
    manta::Session::get().setConsole(&m_console);

    m_updateIndex = 0;
    m_blendLocation = 0.0;
}

ysTexture *mantaray_ui::Application::createTexture(const manta::VectorMap2D *vectorMap) {
    const int width = vectorMap->getWidth();
    const int height = vectorMap->getHeight();
    unsigned char *buffer = new unsigned char[width * height * 4];

    for (int i = 0; i < width; ++i) {
        for (int j = 0; j < height; ++j) {
            const manta::math::Vector v = vectorMap->get(i, j);
            buffer[(j * width + i) * 4 + 0] = (int)(min(std::round(manta::math::getX(v) * 255), 255));
            buffer[(j * width + i) * 4 + 1] = (int)(min(std::round(manta::math::getY(v) * 255), 255));
            buffer[(j * width + i) * 4 + 2] = (int)(min(std::round(manta::math::getZ(v) * 255), 255));
            buffer[(j * width + i) * 4 + 3] = 0xFF;
        }
    }

    ysTexture *newTexture;
    m_engine.GetDevice()->CreateTexture(&newTexture, width, height, buffer);

    delete[] buffer;

    return newTexture;
}

void mantaray_ui::Application::updateImageThread(const manta::VectorMap2D *vectorMap, int index) {
    ysTexture *newTexture = createTexture(vectorMap);

    std::lock_guard<std::mutex> lock(m_textureLock);
    if (m_previewSnapshots.size() > 0) {
        if (m_previewSnapshots.back().index > index) {
            m_engine.GetDevice()->DestroyTexture(newTexture);
            return;
        }
    }

    m_previewSnapshots.push_back({ newTexture, index });

    m_textureUpdateComplete = true;
}

float mantaray_ui::Application::clamp(float s, float left, float right) {
    if (s > right) return right;
    else if (s < left) return left;
    else return s;
}

void mantaray_ui::Application::getBlend(ysTexture **texture0, ysTexture **texture1, float &blend, float &extent) {
    std::lock_guard<std::mutex> lock(m_textureLock);

    extent = 0;

    const int n = m_previewSnapshots.size();
    if (n > 0) {
        *texture0 = nullptr;
        *texture1 = m_previewSnapshots.back().texture;
        blend = 1.0f;
    }
    else {
        *texture1 = nullptr;
    }
}

void mantaray_ui::Application::updateImage() {
    
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

mantaray_ui::BoundingBox mantaray_ui::Application::fitImage(int width, int height, const BoundingBox &extents) {
    const float fit_x = extents.Width() / width;
    const float fit_y = extents.Height() / height;
    const float imageScale = ((fit_x < fit_y) ? fit_x : fit_y);

    return BoundingBox(width * imageScale, height * imageScale)
        .AlignCenterX(extents.CenterX())
        .AlignCenterY(extents.CenterY());
}

void mantaray_ui::Application::process() {
    if (m_fileChangeCount > 0) {
        m_fileChangeDebounce = 0.1f;
        m_fileChangeCount = 0;
        m_debounceTriggered = true;
    }

    if (m_debounceTriggered) {
        m_fileChangeDebounce -= m_engine.GetFrameLength();
        if (m_fileChangeDebounce < 0.0f) {
            m_debounceTriggered = false;

            manta::Session::get().getConsole()->out("File modification detected. Recompiling.\n");
            recompile();
        }
    }

    int mouseX, mouseY;
    m_engine.GetOsMousePos(&mouseX, &mouseY);

    if (m_engine.GetGameWindow()->IsOnScreen(mouseX, mouseY)) {
        m_zoom += (m_engine.GetMouseWheel() - m_lastMouseWheel);
    }

    m_lastMouseWheel = m_engine.GetMouseWheel();
}

void mantaray_ui::Application::render() {
    const int screenWidth = m_engine.GetGameWindow()->GetGameWidth();
    const int screenHeight = m_engine.GetGameWindow()->GetGameHeight();

    m_shaders.SetScreenDimensions((float)screenWidth, (float)screenHeight);
    m_shaders.CalculateUiCamera();
    m_shaders.SetFogNear(10000.0f);
    m_shaders.SetFogFar(10001.0f);

    BoundingBox screenExtents(screenWidth, screenHeight);
    Grid screenGrid(screenExtents, 10, 5, 10.0f);

    BoundingBox terminalArea = screenGrid.GetFullRange(0, 2, 0, 4);
    m_console.setExtents(terminalArea.MarginOffset(-10.0f, -10.0f));

    manta::Session &session = manta::Session::get();
    std::vector<manta::ImagePreviewContainer> previews;
    manta::Session::get().getImagePreviews(previews);

    m_updateTimer -= m_engine.GetFrameLength();

    if (previews.size() > 0 && m_updateTimer <= 0.0f) {
        manta::ImagePreviewContainer &preview = previews.back();
        if (preview.map != nullptr) {
            std::thread updateThread(&Application::updateImageThread, this, preview.map, m_updateIndex++);
            updateThread.detach();
        }
    }

    updateImage();

    m_shaders.ResetLights();

    BoundingBox displayArea = screenGrid.GetFullRange(3, 9, 0, 4);
    drawBox(displayArea, ysColor::srgbiToLinear(0x0B0D10), ysMath::Constants::One, ysMath::Constants::Zero);

    int mouseX, mouseY;
    m_engine.GetOsMousePos(&mouseX, &mouseY);
    const int mouseDx = mouseX - m_lastMouseX;
    const int mouseDy = mouseY - m_lastMouseY;

    m_lastMouseX = mouseX;
    m_lastMouseY = mouseY;

    const float zoomScale = std::pow(0.5, -m_zoom / 240.0f);

    if (m_engine.GetGameWindow()->IsOnScreen(mouseX, mouseY)) {
        if (m_engine.ProcessMouseButtonDown(ysMouse::Button::Left)) {
            m_dragging = true;
        }
    }

    if (m_engine.IsMouseButtonDown(ysMouse::Button::Left) && m_dragging) {
        m_pan = ysMath::Add(m_pan, ysMath::LoadVector(mouseDx / zoomScale, mouseDy / zoomScale));
    }
    else {
        m_dragging = false;
    }

    ysTexture *t0, *t1;
    float blend, extent;
    getBlend(&t0, &t1, blend, extent);

    if (t1 != nullptr) {
        const BoundingBox preview = fitImage(t1->GetWidth(), t1->GetHeight(), displayArea.MarginOffset(-20.0f, -20.0f));
        const BoundingBox outline = preview.MarginOffset(1.0f, 1.0f);

        drawBox(outline, ysColor::srgbiToLinear(0x333333), ysMath::LoadScalar(zoomScale), m_pan);
        if (t0 != nullptr) drawImage(t0, preview, ysMath::LoadVector(1.0f, 1.0f, 1.0f, 1.0f), ysMath::LoadScalar(zoomScale), m_pan);
        drawImage(t1, preview, ysMath::LoadVector(1.0f, 1.0f, 1.0f, blend), ysMath::LoadScalar(zoomScale), m_pan);

        if (m_blendLocation < extent) {
            m_blendLocation += m_engine.GetFrameLength() * 2;

            if (m_blendLocation >= extent) {
                m_blendLocation = extent;
            }
        }
    }

    drawBox(terminalArea.MarginOffset(1.0f, 1.0f), ysColor::srgbiToLinear(0x394351), ysMath::Constants::One, ysMath::Constants::Zero);
    drawBox(terminalArea, ysColor::srgbiToLinear(0x0B0D10), ysMath::Constants::One, ysMath::Constants::Zero);

    if (m_updateTimer < 0.0f) {
        m_updateTimer = 2.0f;
    }

    m_console.render();
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

void mantaray_ui::Application::drawImage(ysTexture *texture, const BoundingBox &box, const ysVector &color, const ysVector &scale, const ysVector &offset) {
    const int wx = m_engine.GetScreenWidth();
    const int wy = m_engine.GetScreenHeight();

    ysMatrix transform = ysMath::MatMult(
        ysMath::ScaleTransform(scale),
        ysMath::TranslationTransform(
            ysMath::LoadVector(
                ysMath::GetX(offset),
                ysMath::GetY(offset))));

    transform = ysMath::MatMult(
        ysMath::TranslationTransform(ysMath::LoadVector(box.CenterX() - wx / 2.0f, box.CenterY() - wy / 2.0f)),
        transform);

    m_shaders.SetBaseColor(color);
    m_shaders.SetLit(false);
    m_shaders.SetColorReplace(false);
    m_shaders.SetDiffuseTexture(texture);
    m_shaders.SetObjectTransform(transform);
    m_shaders.SetScale(box.Width() / 2.0f, box.Height() / 2.0f);
    m_engine.DrawBox(m_shaders.GetRegularFlags());
}

void mantaray_ui::Application::drawBox(
    const BoundingBox &box, 
    const ysVector &color, 
    const ysVector &scale, 
    const ysVector &offset) 
{
    const int wx = m_engine.GetScreenWidth();
    const int wy = m_engine.GetScreenHeight();

    ysMatrix transform = ysMath::MatMult(
        ysMath::ScaleTransform(scale),
        ysMath::TranslationTransform(
            ysMath::LoadVector(
                ysMath::GetX(offset),
                ysMath::GetY(offset))));

    transform = ysMath::MatMult(
        ysMath::TranslationTransform(ysMath::LoadVector(box.CenterX() - wx / 2.0f, box.CenterY() - wy / 2.0f)),
        transform);

    m_shaders.SetBaseColor(color);
    m_shaders.SetLit(false);
    m_shaders.SetColorReplace(true);
    m_shaders.SetDiffuseTexture(nullptr);
    m_shaders.SetObjectTransform(transform);
    m_shaders.SetScale(box.Width() / 2.0f, box.Height() / 2.0f);
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
        FILE_NOTIFY_CHANGE_SECURITY | FILE_NOTIFY_CHANGE_CREATION |
        FILE_NOTIFY_CHANGE_LAST_WRITE |
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
            m_fileChangeCount = m_fileChangeCount + 1;
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
