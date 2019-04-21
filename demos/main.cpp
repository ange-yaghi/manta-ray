#include <demos.h>

using namespace manta_demo;

int main() {
	// Set up all required directories
	createAllDirectories();

	// Run a demo
	constexpr DEMO demo = DEMO::TEAPOT_LAMP;
	constexpr int samples = 1;
	constexpr int resolutionX = 2048 / 16;
	constexpr int resolutionY = 1536 / 16;

	runDemo(demo, samples, resolutionX, resolutionY);

	return 0;
}
