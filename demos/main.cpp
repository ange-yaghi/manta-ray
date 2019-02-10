#include <demos.h>

using namespace manta_demo;

int main() {
	// Set up all required directories
	createAllDirectories();

	// Run a demo
	constexpr DEMO demo = DEMO::STRESS_SPIDERS;
	constexpr int samples = 1;
	constexpr int resolutionX = 2048;
	constexpr int resolutionY = 1536;

	runDemo(demo, samples, resolutionX, resolutionY);

	return 0;
}