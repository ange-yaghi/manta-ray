#include "demos.h"

using namespace manta_demo;

int main() {
    // Set up all required directories
    createAllDirectories();

    // Run a demo
    constexpr DEMO demo = DEMO::SAMSUNG_A8;
    constexpr int samples = 100;
    constexpr int resolutionX = 1920;
    constexpr int resolutionY = 1080;

    runDemo(demo, samples, resolutionX, resolutionY);

    return 0;
}
