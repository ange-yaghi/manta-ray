#include <utils.h>

#include <sstream>
#include <time.h>

manta::math::Vector manta_demo::getColor(int r, int g, int b) {
	return manta::math::loadVector(r / 255.0f, g / 255.0f, b / 255.0f);
}

std::string manta_demo::createUniqueRenderFilename(const char *dir, const char *jobName, int samples) {
	time_t rawTime;
	struct tm timeInfo;
	char buffer[256];

	time(&rawTime);
	localtime_s(&timeInfo, &rawTime);

	strftime(buffer, 256, "%F_T%H_%M_%S", &timeInfo);

	std::stringstream ss;
	ss << samples;

	return std::string(dir) + buffer + std::string(jobName) + "_S" + ss.str() + ".bmp";
}
