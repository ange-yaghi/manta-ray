#include <pch.h>
#include "utilities.h"

#include <fraunhofer_diffraction.h>
#include <image_plane.h>
#include <circular_aperture.h>
#include <square_aperture.h>
#include <polygonal_aperture.h>
#include <image_byte_buffer.h>
#include <jpeg_writer.h>
#include <color.h>
#include <cmf_table.h>
#include <spectrum.h>

using namespace manta;

TEST(FraunhoferTests, FraunhoferSampleTest) {
	CircularAperture aperture;
	aperture.setRadius(5.0f);

	FraunhoferDiffraction::Settings settings;
	FraunhoferDiffraction::setDefaultSettings(&settings);
	settings.maxSamples = 1024;
	settings.frequencyMultiplier = 3;

	FraunhoferDiffraction diff;
	CmfTable colorTable;
	Spectrum sourceSpectrum;
	colorTable.loadCsv(CMF_PATH "xyz_cmf_31.csv");
	sourceSpectrum.loadCsv(CMF_PATH "d65_lighting.csv");

	diff.generate(&aperture, nullptr, 256, 0.15f, &colorTable, &sourceSpectrum, &settings);

	const VectorMap2D *pattern = diff.getDiffractionPattern();
	int width = pattern->getWidth();
	int height = pattern->getHeight();
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			math::Vector v = pattern->get(i, j);
			math::real r, g, b;
			r = math::getX(v);
			g = math::getY(v);
			b = math::getZ(v);

			EXPECT_GE(r, 0.0f);
			EXPECT_GE(g, 0.0f);
			EXPECT_GE(b, 0.0f);
		}
	}

	math::Vector s = diff.samplePattern(0.0f, 0.0f);
	math::Vector s2 = diff.samplePattern(0.1f, 0.1f);

	math::real mag_s1 = math::getScalar(math::magnitude(s));
	math::real mag_s2 = math::getScalar(math::magnitude(s2));

	EXPECT_TRUE(mag_s1 > mag_s2);

	diff.destroy();
	colorTable.destroy();
	sourceSpectrum.destroy();
}
