#include <demos.h>

using namespace manta_demo;

int main() {
	// Set up all required directories
	createAllDirectories();

	// Run a demo

	//complexRoomDemo(40, 1024 * 2, 768 * 2);
	//penDemo(22000, 1024 * 2, 768 * 2);
	//stressSpidersDemo(4500, 1024 * 2, 768 * 2);
	//simpleRoomDemo(1, 1024*2, 768*2);
	//boxCityDemo(100, 1024*2, 768*2);
	//teapotLampDemo(200, 1024*2, 768*2);
	blocksDemo(100, 1024 * 2, 768 * 2);

	return 0;
}