#include "../include/application.h"

#include <iostream>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    (void)nCmdShow;
    (void)lpCmdLine;
    (void)hPrevInstance;

    mantaray_ui::Application app;
    app.initialize((void *)&hInstance, ysContextObject::DeviceAPI::DirectX11);
    app.run();
    app.destroy();

    return 0;
}
