#include <pch.h>

#include "utilities.h"

#include "../include/image_plane.h"
#include "../include/box_filter.h"

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

    BoxFilter filter;
    filter.setExtents(math::Vector2(0.5f, 0.5f));
    StackAllocator stack;
    stack.initialize(10 * KB);
    imagePlane.setFilter(&filter);
    imagePlane.processSamples(samples, 4, &stack);
    imagePlane.normalize();

    imagePlane.destroy();
}
