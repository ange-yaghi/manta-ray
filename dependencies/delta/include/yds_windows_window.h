#ifndef YDS_WINDOWS_WINDOW_H
#define YDS_WINDOWS_WINDOW_H

#include "yds_window.h"

#include <Windows.h>

class ysWindowsWindowSystem;

class ysWindowsWindow : public ysWindow {
    friend ysWindowsWindowSystem;

public:
    ysWindowsWindow();
    virtual ~ysWindowsWindow();

    // Overrides

    virtual ysError InitializeWindow(ysWindow *parent, const char *title, WindowStyle style, int x, int y, int width, int height, ysMonitor *monitor);
    virtual ysError InitializeWindow(ysWindow *parent, const char *title, WindowStyle style, ysMonitor *monitor);

    virtual bool SetWindowStyle(WindowStyle style);

    virtual void ScreenToLocal(int &x, int &y) const;

    virtual bool IsVisible();

    virtual int GetScreenWidth() const;
    virtual int GetScreenHeight() const;

public:
    // Windows Specific

    // Set the windows instance to use
    void SetInstance(HINSTANCE instance) { m_instance = instance; }
    HINSTANCE GetInstance() const { return m_instance; }

    // Get Win32 window handle
    HWND GetWindowHandle() const { return m_hwnd; }

    int GetWindowsStyle() const;

protected:
    // Abstraction Layer

    virtual void Close();
    virtual void SetTitle(const char *title);
    virtual void SetState(WindowState state = WindowState::Visible);

    virtual void AL_SetSize(int width, int height);
    virtual void AL_SetLocation(int x, int y);

protected:
    ATOM RegisterWindowsClass();

    HINSTANCE m_instance;
    HWND m_hwnd;
};

#endif /* YDS_WINDOWS_WINDOW_H */
