#ifndef UTILS_H
#define UTILS_H

#include <manta_math.h>

namespace manta_demo {

	manta::math::Vector getColor(int r, int g, int b);
	std::string createUniqueRenderFilename(const char *dir, const char *jobName, int samples);

} /* namespace manta_demo */

#endif /* UTILS_H */
