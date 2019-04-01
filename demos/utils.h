#ifndef UTILS_H
#define UTILS_H

#include <image_plane.h>
#include <manta_math.h>

#include <string>

namespace manta_demo {

	void createAllDirectories();
	manta::math::Vector getColor(int r, int g, int b, manta::math::real gamma = (manta::math::real)2.2);
	std::string createUniqueRenderFilename(const char *jobName, int samples);
	void writeJpeg(const char *fname, manta::ImagePlane *sceneBuffer, int quality = 95);
	void editImage(manta::ImagePlane *sceneBuffer, const std::string &outputFname);

} /* namespace manta_demo */

#endif /* UTILS_H */
