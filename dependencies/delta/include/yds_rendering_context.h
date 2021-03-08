#ifndef YDS_RENDERING_CONTEXT_H
#define YDS_RENDERING_CONTEXT_H

#include "yds_context_object.h"

#include "yds_window.h"
#include "yds_window_system.h"
#include "yds_render_target.h"

class ysDevice;

class ysRenderingContext : public ysContextObject {
    friend ysDevice;

public:
    enum class ContextMode {
        Windowed,
        Fullscreen,
        Undefined
    };

public:
    ysRenderingContext();
    ysRenderingContext(DeviceAPI API, ysWindowSystem::Platform platform);
    virtual ~ysRenderingContext();

    ysWindow *GetWindow() const { return m_targetWindow; }
    ysRenderTarget *GetAttachedRenderTarget() const { return m_attachedRenderTarget; }

    ContextMode GetCurrentMode() const { return m_currentMode; }
    ysWindowSystem::Platform GetPlatform() const { return m_platform; }

protected:
    ysWindow *m_targetWindow;
    ysRenderTarget *m_attachedRenderTarget;

    ContextMode m_currentMode;

    ysWindowSystem::Platform m_platform;
};

#endif /* YDS_RENDERING_CONTEXT_H */
