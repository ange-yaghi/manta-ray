#include "demos.h"

using namespace manta_demo;

int main() {
	// Set up all required directories
	createAllDirectories();

	// Run a demo
	constexpr DEMO demo = DEMO::BOX_CITY;
	constexpr int samples = 100;
	constexpr int resolutionX = 2048 / 2;
	constexpr int resolutionY = 1536 / 2;

	runDemo(demo, samples, resolutionX, resolutionY);

	return 0;
}
