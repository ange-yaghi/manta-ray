#include <pch.h>
#include "utilities.h"

#include <fraunhofer_diffraction.h>
#include <image_plane.h>
#include <circular_aperture.h>
#include <square_aperture.h>
#include <polygonal_aperture.h>
#include <image_byte_buffer.h>
#include <jpeg_writer.h>

using namespace manta;

TEST(FraunhoferTests, FraunhoferSampleTest) {
	CircularAperture aperture;
	aperture.setRadius(5.0f);

	FraunhoferDiffraction::Settings settings;
	FraunhoferDiffraction::defaultSettings(&settings);
	settings.maxSamples = 1024;
	settings.frequencyMultiplier = 3;

	FraunhoferDiffraction diff;
	diff.generate(&aperture, 64, 0.1f, &settings);

	math::Vector s = diff.samplePattern(0.0, 0.0);
	math::Vector2 l = diff.getPerturbance(0, 0);

	EXPECT_NEAR(l.x, 0.0f, 1E-4);
	EXPECT_NEAR(l.y, 0.0f, 1E-4);

	CHECK_VEC_EQ(s, math::loadVector(1.0, 1.0, 1.0), 1E-3);

	math::Vector s2 = diff.samplePattern(0.1, 0.1);

	math::real mag_s1 = math::getScalar(math::magnitude(s));
	math::real mag_s2 = math::getScalar(math::magnitude(s2));

	EXPECT_TRUE(mag_s1 > mag_s2);

	diff.destroy();
}
