#include <pch.h>

#include "utilities.h"

#include "../include/image_plane.h"

using namespace manta;

TEST(ImagePlaneTests, ImagePlaneSanityCheck) {
    ImagePlane imagePlane;
    imagePlane.initialize(2, 2);

    ImageSample samples[4];
    samples[0].imagePlaneLocation = math::Vector2(0.0f, 0.0f);
    samples[0].intensity = math::loadScalar(0.5f);
    samples[1].imagePlaneLocation = math::Vector2(1.0f, 0.0f);
    samples[1].intensity = math::loadScalar(0.5f);
    samples[2].imagePlaneLocation = math::Vector2(0.0f, 1.0f);
    samples[2].intensity = math::loadScalar(0.5f);
    samples[3].imagePlaneLocation = math::Vector2(1.0f, 1.0f);
    samples[3].intensity = math::loadScalar(0.5f);

    imagePlane.processSamples(samples, 4, nullptr);
    imagePlane.normalize();

    imagePlane.destroy();
}
