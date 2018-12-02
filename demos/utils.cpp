#include <utils.h>

#include <image_handling.h>

#include <sstream>
#include <time.h>
#include <iostream>

manta::math::Vector manta_demo::getColor(int r, int g, int b, manta::math::real gamma) {
	manta::math::real rr = r / 255.0;
	manta::math::real rg = g / 255.0;
	manta::math::real rb = b / 255.0;

	rr = pow(rr, gamma);
	rg = pow(rg, gamma);
	rb = pow(rb, gamma);

	manta::math::Vector v = manta::math::loadVector(rr, rg, rb);
	return v;
}

std::string manta_demo::createUniqueRenderFilename(const char *jobName, int samples) {
	time_t rawTime;
	struct tm timeInfo;
	char buffer[256];

	time(&rawTime);
	localtime_s(&timeInfo, &rawTime);

	strftime(buffer, 256, "%F_T%H_%M_%S", &timeInfo);

	std::stringstream ss;
	ss << samples;

	return std::string(buffer) + "_" + std::string(jobName) + "_S" + ss.str();
}

void manta_demo::editImage(manta::SceneBuffer *sceneBuffer, const std::string &outputFname) {
	manta::SaveImageData(sceneBuffer->getBuffer(), sceneBuffer->getWidth(), sceneBuffer->getHeight(), outputFname.c_str());

	// Create a temporary scene buffer
	manta::SceneBuffer temp;

	std::cout << "**** Image Editing Utility ****" << std::endl;
	while (true) {
		char command;
		std::cout << "Edit image? (y/n) ";
		std::cin >> command;
		if (command == 'y' || command == 'Y') {
			sceneBuffer->clone(&temp);

			manta::math::real scale;

			std::cout << "Enter scale factor: ";
			std::cin >> scale;

			temp.scale(scale);
			manta::SaveImageData(temp.getBuffer(), temp.getWidth(), temp.getHeight(), outputFname.c_str());

			temp.destroy();
		}
		else {
			break;
		}
	}
}
