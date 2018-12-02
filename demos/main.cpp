#include <demos.h>

using namespace manta_demo;

int main() {
	//simpleRoomDemo(10, 1024*2, 768*2); //1024, 768
	//teapotDemo(1, 500, 500);
	teapotLampDemo(100, 1024*.1, 768*.1); // 10 for issue replication 1024, 768
	//cubeTestDemo(1, 1024, 768);

	return 0;
}