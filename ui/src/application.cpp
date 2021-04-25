#include "../include/application.h"

#include "../../include/os_utilities.h"
#include "../../include/vector_map_2d.h"
#include "../../include/manta_math.h"
#include "../../include/preview_node.h"
#include "../include/grid.h"

#include <Windows.h>

mantaray_ui::Application *mantaray_ui::Application::s_application = nullptr;

const ysVector mantaray_ui::Application::StandardBlue = ysColor::srgbiToSrgb(0x00A4E9);
const ysVector mantaray_ui::Application::StandardRed = ysColor::srgbiToSrgb(0xFF1549);
const ysVector mantaray_ui::Application::StandardYellow = ysColor::srgbiToSrgb(0xFF9F15);

mantaray_ui::Application::Application() {
    m_inputFile = "../../workspace/ui_test/script/maserati.mr";
    m_fileWatcherThread = nullptr;
    m_fileChangeCount = 0;
    m_fileChangeDebounce = 0.0f;
    m_debounceTriggered = false;
    m_updateTimer = 0.0f;

    m_lastMouseX = 0;
    m_lastMouseY = 0;

    m_dragging = false;
    m_textureUpdateComplete = true;
    m_activePreview = nullptr;

    m_manualCompileTriggered = false;
    m_compileTriggered = false;
    m_cancelTriggered = false;

    m_currentState = State::Ready;
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

    m_currentState = State::Undefined;
    fsmChangeState(State::Ready);
}

float mantaray_ui::Application::clamp(float s, float left, float right) {
    if (s > right) return right;
    else if (s < left) return left;
    else return s;
}

void mantaray_ui::Application::compile() {
    m_compilerThread = new CompilerThread;
    m_compilerThread->initialize();
    m_compilerThread->compileAndExecute(piranha::IrPath(m_inputFile));
}

mantaray_ui::BoundingBox mantaray_ui::Application::fitImage(int width, int height, const BoundingBox &extents) {
    const float fit_x = extents.Width() / width;
    const float fit_y = extents.Height() / height;
    const float imageScale = ((fit_x < fit_y) ? fit_x : fit_y);

    return BoundingBox(width * imageScale, height * imageScale)
        .AlignCenterX(extents.CenterX())
        .AlignCenterY(extents.CenterY());
}

void mantaray_ui::Application::fsm() {
    State nextState = m_currentState;
    if (m_currentState == State::Ready) {
        if (m_compileTriggered || m_manualCompileTriggered) {
            nextState = State::CompilingAndExecuting;
        }
    }
    else if (m_currentState == State::CompilingAndExecuting) {
        if (m_compileTriggered || m_manualCompileTriggered || m_cancelTriggered) {
            nextState = State::Killing;
        }
        else if (m_compilerThread->isExecutionComplete()) {
            nextState = State::Finalizing;
        }
    }
    else if (m_currentState == State::Killing) {
        if (m_compilerThread->isExecutionComplete()) {
            nextState = State::Destroying;
        }
    }
    else if (m_currentState == State::Finalizing) {
        bool doneUpdating = true;
        for (Preview *preview : m_previews) {
            if (preview->isUpdating()) {
                doneUpdating = false;
                break;
            }
        }

        if (doneUpdating) {
            nextState = State::Destroying;
        }
    }
    else if (m_currentState == State::Destroying) {
        if (m_compilerThread->isDestroyed()) {
            nextState = State::Ready;
        }
    }

    fsmChangeState(nextState);
}

void mantaray_ui::Application::fsmChangeState(State nextState) {
    if (nextState == State::Ready) {
        if (m_currentState == State::Destroying) {
            manta::Session::get().getConsole()->out("All memory freed.\n", StandardBlue);

            delete m_compilerThread;
            m_compilerThread = nullptr;

            m_console.out("Listening...\n", StandardYellow);
        }
        else if (m_currentState == State::Undefined) {
            m_console.out("Listening...\n", StandardYellow);
        }
    }
    else if (nextState == State::CompilingAndExecuting) {
        if (m_currentState == State::Ready) {
            if (m_compileTriggered) {
                manta::Session::get().getConsole()->out("File modification detected. Recompiling.\n", StandardBlue);
            }
            else if (m_manualCompileTriggered) {
                manta::Session::get().getConsole()->out("Enter key press detected. Recompiling.\n", StandardBlue);
            }

            clearPreviews();
            compile();

            m_compileTriggered = false;
            m_manualCompileTriggered = false;
        }
    }
    else if (nextState == State::Killing) {
        if (m_currentState == State::CompilingAndExecuting) {
            if (m_compileTriggered) {
                manta::Session::get().getConsole()->out("File modification detected. Stopping job.\n", StandardRed);
            }
            else if (m_manualCompileTriggered) {
                manta::Session::get().getConsole()->out("Enter key press detected. Stopping job.\n", StandardRed);
            }
            else if (m_cancelTriggered) {
                manta::Session::get().getConsole()->out("Render cancelled. Stopping job.\n", StandardRed);
            }

            m_compilerThread->kill();
            m_compilerThread->setContinue(true);

            m_cancelTriggered = false;
        }
    }
    else if (nextState == State::Finalizing) {
        if (m_currentState == State::CompilingAndExecuting) {
            manta::Session::get().getConsole()->out("Execution complete. Finalizing.\n", StandardBlue);

            for (Preview *preview : m_previews) {
                preview->update(true);
            }
        }
    }
    else if (nextState == State::Destroying) {
        if (m_currentState == State::Killing) {
            m_compilerThread->setContinue(true);
            manta::Session::get().getConsole()->out("Job successfully stopped. Freeing memory.\n", StandardBlue);
        }
        else if (m_currentState == State::Finalizing) {
            m_compilerThread->setContinue(true);
            manta::Session::get().getConsole()->out("All previews finalized. Freeing memory.\n", StandardBlue);
        }
    }

    m_currentState = nextState;
}

mantaray_ui::Preview *mantaray_ui::Application::getPreviewForNode(manta::PreviewNode *node) {
    for (Preview *preview : m_previews) {
        if (preview->getPreviewNode() == node) {
            return preview;
        }
    }

    return nullptr;
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
            m_compileTriggered = true;
        }
    }

    if (m_engine.GetGameWindow()->IsActive()) {
        if (m_engine.ProcessKeyDown(ysKey::Code::Return)) {
            m_manualCompileTriggered = true;
        }
        else if (m_engine.ProcessKeyDown(ysKey::Code::Escape)) {
            m_cancelTriggered = true;
        }
        else if (m_engine.ProcessKeyDown(ysKey::Code::Down)) {
            if (m_activePreview != nullptr) {
                const int newIndex = min(m_activePreview->getIndex() + 1, (int)m_previews.size() - 1);
                m_activePreview = m_previews[newIndex];
            }
        }
        else if (m_engine.ProcessKeyDown(ysKey::Code::Up)) {
            if (m_activePreview != nullptr) {
                const int newIndex = max(m_activePreview->getIndex() - 1, 0);
                m_activePreview = m_previews[newIndex];
            }
        }
    }

    fsm();

    std::vector<manta::PreviewNode *> nodes = manta::Session::get().getPreviews();
    if (m_currentState != State::Destroying) {
        for (manta::PreviewNode *node : nodes) {
            if (getPreviewForNode(node) == nullptr) {
                Preview *newPreview = new Preview;
                newPreview->initialize(&m_engine, (int)m_previews.size());
                newPreview->setPreviewNode(node);
                newPreview->update(true);

                m_previews.push_back(newPreview);

                m_activePreview = newPreview;
            }
        }
    }

    if (m_currentState == State::CompilingAndExecuting) {
        if (m_previews.size() > 0) {
            if (m_updateTimer <= 0.0f) {
                m_activePreview->update(false);
            }
        }
    }

    for (Preview *preview : m_previews) {
        preview->clean();
    }

    if (m_updateTimer < 0.0f) {
        m_updateTimer = 2.0f;
    }
}

void mantaray_ui::Application::render() {
    const float screenWidth = (float)m_engine.GetGameWindow()->GetGameWidth();
    const float screenHeight = (float)m_engine.GetGameWindow()->GetGameHeight();

    m_shaders.SetScreenDimensions(screenWidth, screenHeight);
    m_shaders.CalculateUiCamera();
    m_shaders.SetFogNear(10000.0f);
    m_shaders.SetFogFar(10001.0f);

    BoundingBox screenExtents(screenWidth, screenHeight);
    Grid screenGrid(screenExtents, 10, 5, 10.0f);

    BoundingBox terminalArea = screenGrid.GetFullRange(0, 2, 0, 4).PixelPerfect();
    m_console.setExtents(terminalArea.MarginOffset(-10.0f, -10.0f));

    m_updateTimer -= m_engine.GetFrameLength();

    m_shaders.ResetLights();

    BoundingBox fullDisplayArea = screenGrid.GetFullRange(3, 9, 0, 4);
    BoundingBox displayArea = BoundingBox(fullDisplayArea.Width(), fullDisplayArea.Height() - 50.0f)
        .AlignBottom(fullDisplayArea.Bottom())
        .AlignLeft(fullDisplayArea.Left())
        .PixelPerfect();
    BoundingBox statusArea = BoundingBox(fullDisplayArea.Width(), 50.0f)
        .AlignTop(fullDisplayArea.Top())
        .AlignLeft(fullDisplayArea.Left())
        .PixelPerfect();

    drawBox(fullDisplayArea, ysColor::srgbiToLinear(0x0B0D10), ysMath::Constants::One, ysMath::Constants::Zero);

    int mouseX, mouseY;
    m_engine.GetOsMousePos(&mouseX, &mouseY);
    const int mouseDx = mouseX - m_lastMouseX;
    const int mouseDy = mouseY - m_lastMouseY;

    m_lastMouseX = mouseX;
    m_lastMouseY = mouseY;

    const bool mouseOnScreen = m_engine.GetGameWindow()->IsOnScreen(mouseX, mouseY);
    if (mouseOnScreen) {
        if (m_engine.ProcessMouseButtonDown(ysMouse::Button::Left)) {
            m_dragging = true;
        }
    }

    if (m_activePreview != nullptr) {
        if (mouseOnScreen) {
            m_activePreview->zoom(m_engine.GetMouseWheel() - m_lastMouseWheel);

            if (m_engine.ProcessMouseButtonDown(ysMouse::Button::Right)) {
                m_activePreview->resetPan();
                m_activePreview->resetZoom();
            }
        }

        const float zoomScale = std::pow(0.5f, -m_activePreview->getZoom() / 240.0f);
        if (m_engine.IsMouseButtonDown(ysMouse::Button::Left) && m_dragging) {
            m_activePreview->pan(ysMath::LoadVector(mouseDx / zoomScale, mouseDy / zoomScale));
        }
        else {
            m_dragging = false;
        }

        ysTexture *texture = m_activePreview->getCurrentPreview();

        if (texture != nullptr) {
            const BoundingBox preview = fitImage(texture->GetWidth(), texture->GetHeight(), displayArea.MarginOffset(-20.0f, -20.0f));
            const BoundingBox outline = preview.MarginOffset(1.0f, 1.0f);

            drawBox(outline, ysColor::srgbiToLinear(0x333333), ysMath::LoadScalar(zoomScale), m_activePreview->getPan());
            drawImage(texture, preview, ysMath::LoadVector(1.0f, 1.0f, 1.0f, 1.0f), ysMath::LoadScalar(zoomScale), m_activePreview->getPan());
        }
    }

    drawBox(BoundingBox(statusArea).AlignBottom(statusArea.Bottom() - 1), ysColor::srgbiToLinear(0x394351), ysMath::Constants::One, ysMath::Constants::Zero);
    drawBox(statusArea, ysColor::srgbiToLinear(0x0B0D10), ysMath::Constants::One, ysMath::Constants::Zero);
    m_textRenderer.SetColor(ysColor::srgbiToSrgb(0x00A4E9));
    m_textRenderer.RenderText(
        (m_activePreview != nullptr)
            ? m_activePreview->getTitle()
            : "No Preview Available",
        statusArea.Left() - (screenWidth / 2.0f) + 20.0f,
        statusArea.Top() - 16.0f - (screenHeight / 2.0f) - 20.0f,
        16.0f);

    m_lastMouseWheel = m_engine.GetMouseWheel();

    drawBox(terminalArea.MarginOffset(1.0f, 1.0f), ysColor::srgbiToLinear(0x394351), ysMath::Constants::One, ysMath::Constants::Zero);
    drawBox(terminalArea, ysColor::srgbiToLinear(0x0B0D10), ysMath::Constants::One, ysMath::Constants::Zero);

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

void mantaray_ui::Application::clearPreviews() {
    for (Preview *preview : m_previews) {
        preview->destroy();
        delete preview;
    }

    m_previews.clear();
    m_activePreview = nullptr;
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
