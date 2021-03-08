#ifndef YDS_WINDOW_EVENT_HANDLER_H
#define YDS_WINDOW_EVENT_HANDLER_H

#include "yds_base.h"

// Window Event Handler
class ysWindowEventHandler : public ysObject {
    friend class ysWindow;

public:
    ysWindowEventHandler();
    virtual ~ysWindowEventHandler();

    virtual void OnMoveWindow(int x, int y) { (void)x; (void)y; }
    virtual void OnResizeWindow(int w, int h) { (void)w; (void)h; }
    virtual void OnActivate() { /* void */ }
    virtual void OnDeactivate() { /* void */ }
    virtual void OnCloseWindow() { /* void */ }
    virtual void OnKeyDown(int key) { (void)key; }

protected:
    inline ysWindow *GetWindow() { return m_window; }
     
private:
    ysWindow *m_window;
};

#endif /* YDS_WINDOW_EVENT_HANDLER_H */
