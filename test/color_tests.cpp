#include <pch.h>

#include <utilities.h>

#include <color.h>
#include <rgb_space.h>
#include <cmf_table.h>
#include <manta_math.h>
#include <spectrum.h>

using namespace manta;

TEST(ColorTests, ColorSpaceBasicTest) {
	CmfTable table;
	bool result = table.loadCsv(CMF_PATH "xyz_cmf_31.csv");

	math::Vector rgb = table.pureToRgb(470);

	EXPECT_TRUE(result);

	table.destroy();
}

TEST(ColorTests, ColorSpaceD65LightingTest) {
	bool result;

	CmfTable table;
	result = table.loadCsv(CMF_PATH "xyz_cmf_31.csv");
	EXPECT_TRUE(result);

	Spectrum spectrum;
	result = spectrum.loadCsv(CMF_PATH "d65_lighting.csv");
	EXPECT_TRUE(result);

	EXPECT_DOUBLE_EQ(spectrum.getStartWavelength(), 300);
	EXPECT_DOUBLE_EQ(spectrum.getEndWavelength(), 830);

	ColorXyz d65 = table.spectralToXyz(&spectrum);
	d65 = d65 * (100 / d65.y);

	EXPECT_NEAR(d65.x, 95.047, 1E-1);
	EXPECT_NEAR(d65.y, 100.0, 1E-1);
	EXPECT_NEAR(d65.z, 108.883, 1E-1);

	math::Vector rgb = table.xyzToRgb(d65 * (1 / 100.0f));

	spectrum.destroy();
	table.destroy();
}