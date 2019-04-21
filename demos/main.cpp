#include <demos.h>

using namespace manta_demo;

int main() {
	// Set up all required directories
	createAllDirectories();

	// Run a demo
	constexpr DEMO demo = DEMO::TEAPOT_LAMP;
	constexpr int samples = 8000;
	constexpr int resolutionX = 2048 / 1;
	constexpr int resolutionY = 1536 / 1;

	runDemo(demo, samples, resolutionX, resolutionY);

	return 0;
}
