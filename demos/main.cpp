#include <demos.h>

using namespace manta_demo;

int main() {
	// Set up all required directories
	createAllDirectories();

	// Run a demo

	//complexRoomDemo(40, 1024 * 2, 768 * 2);
	penDemo(22000, 1024 * 2, 768 * 2);
	//stressSpidersDemo(1, 1024 * 2, 768 * 2);
	//simpleRoomDemo(1, 1024*2, 768*2);
	//boxCityDemo(100, 1024*2, 768*2);
	//teapotDemo(1, 500, 500);
	//teapotLampDemo(1, 1024*2, 768*2);
	//cubeTestDemo(1, 1024, 768);
	//blocksDemo(17500, 1024 * 2, 768 * 2);

	return 0;
}