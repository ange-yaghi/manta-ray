#include <pch.h>

#include "utilities.h"

#include <mipmap.h>
#include <scalar_map_2d.h>

#include <manta_math.h>

using namespace manta;

TEST(MipmapTests, MipmapSanityCheck) {
	Mipmap<RealMap2D, math::real> map;
	RealMap2D source;

	// Generate simple map
	source.initialize(256, 256);
	for (int i = 0; i < 256; i++) {
		for (int j = 0; j < 256; j++) {
			if (i % 2 == 0 && j % 2 == 0) {
				source.set(4.0f, i, j);
			}
		}
	}

	map.initialize(&source);

	EXPECT_EQ(map.getLevels(), 9);

	int levels = map.getLevels();
	int width = 1;
	for (int i = 0; i < levels; i++) {
		for (int x = 0; x < width; x++) {
			for (int y = 0; y < width; y++) {
				math::real sx = x / (math::real)width;
				math::real sy = y / (math::real)width;
				math::real dx = 1 / (math::real)width;
				math::real dy = 1 / (math::real)width;

				math::real v = map.sample(sx, sy, dx);
				EXPECT_NEAR(v, 1.0f, 1E-3);
			}
		}

		width <<= 1;
	}

	map.destroy();
	source.destroy();
}
