#include <demos.h>

void manta_demo::runDemo(DEMO demo, int samplesPerPixel, int resolutionX, int resolutionY) {
	switch (demo) {
	case DEMO::BLOCKS:
		blocksDemo(samplesPerPixel, resolutionX, resolutionY);
		break;
	case DEMO::BOX_CITY:
		boxCityDemo(samplesPerPixel, resolutionX, resolutionY);
		break;
	case DEMO::COMPLEX_ROOM:
		complexRoomDemo(samplesPerPixel, resolutionX, resolutionY);
		break;
	case DEMO::PEN:
		penDemo(samplesPerPixel, resolutionX, resolutionY);
		break;
	case DEMO::SIMPLE_ROOM:
		simpleRoomDemo(samplesPerPixel, resolutionX, resolutionY);
		break;
	case DEMO::STRESS_SPIDERS:
		stressSpidersDemo(samplesPerPixel, resolutionX, resolutionY);
		break;
	case DEMO::TEAPOT_LAMP:
		teapotLampDemo(samplesPerPixel, resolutionX, resolutionY);
		break;
	case DEMO::SAMSUNG_A8:
		samsungA8Demo(samplesPerPixel, resolutionX, resolutionY);
		break;
	case DEMO::MATERIAL_PREVIEW:
		materialVisualizer(samplesPerPixel, resolutionX, resolutionY);
		break;
	case DEMO::STOCK_SCENE:
		stockSceneDemo(samplesPerPixel, resolutionX, resolutionY);
		break;
	case DEMO::APERTURE_DEMO:
		apertureDemo(samplesPerPixel, resolutionX, resolutionY);
		break;
	default:
		std::cout << "Unknown/undefined demo specified" << std::endl;
		break;
	}
}
