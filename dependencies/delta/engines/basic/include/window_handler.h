#ifndef DELTA_BASIC_DELTA_WINDOW_HANDLER_H
#define DELTA_BASIC_DELTA_WINDOW_HANDLER_H

#include "delta_core.h"

namespace dbasic {

    class DeltaEngine;

    class WindowHandler : public ysWindowEventHandler {
    public:
        WindowHandler();
        virtual ~WindowHandler();

        void Initialize(ysDevice *device, ysRenderingContext *context, DeltaEngine *engine);

        virtual void OnResizeWindow(int width, int height);

    protected:
        ysDevice *m_device;
        ysRenderingContext *m_context;
        DeltaEngine *m_engine;
    };

} /* namesapce dbasic */

#endif /* DELTA_BASIC_DELTA_WINDOW_HANDLER_H */
