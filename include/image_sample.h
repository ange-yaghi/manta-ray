#ifndef MANTARAY_IMAGE_SAMPLE_H
#define MANTARAY_IMAGE_SAMPLE_H

#include "manta_math.h"

namespace manta {

    struct ImageSample {
        math::Vector intensity;
        math::Vector2 imagePlaneLocation;
    };

} /* namespace manta */

#endif /* MANTARAY_IMAGE_SAMPLE_H */
