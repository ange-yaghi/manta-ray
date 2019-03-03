#include <demos.h>

using namespace manta_demo;

int main() {
	// Set up all required directories
	createAllDirectories();

	// Run a demo
	constexpr DEMO demo = DEMO::MATERIAL_PREVIEW;
	constexpr int samples = 500;
	constexpr int resolutionX = 2048;
	constexpr int resolutionY = 1536;

	runDemo(demo, samples, resolutionX, resolutionY);

	return 0;
}