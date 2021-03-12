#ifndef YDS_WINDOW_H
#define YDS_WINDOW_H

#include "yds_window_system_object.h"
#include "yds_monitor.h"

class ysWindowEventHandler;
class ysWindowSystem;

class ysWindow : public ysWindowSystemObject {
public:
    static const int MAX_NAME_LENGTH = 256;

    enum class WindowState {
        Visible,
        Hidden,
        Maximized,
        Minimized,
        Closed,
        Unknown
    };

    enum class WindowStyle {
        Windowed,
        Fullscreen,
        Popup,
        Unknown
    };

public:
    ysWindow();
    ysWindow(Platform platform);
    virtual ~ysWindow();

    /* Interface */
    virtual ysError InitializeWindow(ysWindow *parent, const char *title, WindowStyle style, int x, int y, int width, int height, ysMonitor *monitor);
    virtual ysError InitializeWindow(ysWindow *parent, const char *title, WindowStyle style, ysMonitor *monitor);

    virtual void Close() { SetState(WindowState::Closed); }
    virtual void SetState(WindowState state = WindowState::Visible) { m_windowState = state; }

    void RestoreWindow();

    int GetWidth() const { return m_width; }
    int GetHeight() const { return m_height; }
    virtual int GetScreenWidth() const { return m_width; }
    virtual int GetScreenHeight() const { return m_height; }
    WindowStyle GetWindowStyle() const { return m_windowStyle; }
    ysMonitor *GetMonitor() const { return m_monitor; }

    const int GetGameWidth() const;
    const int GetGameHeight() const;

    int GetPositionX() const { return m_locationx; }
    int GetPositionY() const { return m_locationy; }

    bool IsOnScreen(int x, int y) const;
    virtual void ScreenToLocal(int &x, int &y) const { /* void */ }

    bool IsOpen() const;
    virtual bool IsActive();
    virtual bool IsVisible();

    virtual void SetScreenSize(int width, int height);
    virtual void SetWindowSize(int width, int height);
    virtual void SetLocation(int x, int y);
    virtual void SetTitle(const char *title);

    virtual void StartResizing() { m_resizing = true; }
    virtual void EndResizing() { m_resizing = false; }
    bool IsResizing() const { return m_resizing; }

    virtual bool SetWindowStyle(WindowStyle style);

    void AttachEventHandler(ysWindowEventHandler *handler);

    void SetGameResolutionScale(float scale);

    void SetGameResolutionScaleHorizontal(float scale);
    float GetGameResolutionScaleHorizontal() const { return m_gameResolutionScaleHorizontal; }

    void SetGameResolutionScaleVertical(float scale);
    float GetGameResolutionScaleVertical() const { return m_gameResolutionScaleVertical; }

public:
    // Handlers
    void OnMoveWindow(int x, int y);
    void OnResizeWindow(int w, int h);
    void OnActivate();
    void OnDeactivate();
    void OnCloseWindow();
    void OnKeyDown(int key);

protected:
    // Abstraction Layer
    virtual void AL_SetSize(int width, int height);
    virtual void AL_SetLocation(int x, int y);

protected:
    /* Common Members */

    // Window width
    int m_width;

    // Window height
    int m_height;

    // Location x
    int m_locationx;

    // Location y
    int m_locationy;

    // Title
    char m_title[MAX_NAME_LENGTH];

    // Current Window State
    WindowState m_windowState;

    // Window Style
    WindowStyle m_windowStyle;

    // Parent Window
    ysWindow *m_parent;

    // The attached monitor for use in fullscreen
    ysMonitor *m_monitor;

    // Window Active
    bool m_active;

    // Status flag indicating whether the window is resizing
    bool m_resizing;

    float m_gameResolutionScaleHorizontal;
    float m_gameResolutionScaleVertical;

protected:
    // Event handler
    ysWindowEventHandler *m_eventHandler;
};

#endif /* YDS_WINDOW_H */
