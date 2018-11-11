#ifndef TEST_UTILITIES_H
#define TEST_UTILITIES_H

#define CHECK_VEC(v, ex, ey, ez, ew) {\
		float x = math::getX((v)); \
		float y = math::getY((v)); \
		float z = math::getZ((v)); \
		float w = math::getW((v)); \
		EXPECT_NEAR(x, (ex), 1E-7); \
		EXPECT_NEAR(y, (ey), 1E-7); \
		EXPECT_NEAR(z, (ez), 1E-7); \
		EXPECT_NEAR(w, (ew), 1E-7); \
	}

#define CHECK_QUAT(v, ew, ex, ey, ez) {\
		float x = math::getQuatX((v)); \
		float y = math::getQuatY((v)); \
		float z = math::getQuatZ((v)); \
		float w = math::getQuatW((v)); \
		EXPECT_NEAR(x, (ex), 1E-7); \
		EXPECT_NEAR(y, (ey), 1E-7); \
		EXPECT_NEAR(z, (ez), 1E-7); \
		EXPECT_NEAR(w, (ew), 1E-7); \
	}

#define CHECK_QUAT_EQ(v, ev) {\
		float x = math::getQuatX((v)); \
		float y = math::getQuatY((v)); \
		float z = math::getQuatZ((v)); \
		float w = math::getQuatW((v)); \
		float ex = math::getQuatX((ev)); \
		float ey = math::getQuatY((ev)); \
		float ez = math::getQuatZ((ev)); \
		float ew = math::getQuatW((ev)); \
		EXPECT_NEAR(x, (ex), 1E-7); \
		EXPECT_NEAR(y, (ey), 1E-7); \
		EXPECT_NEAR(z, (ez), 1E-7); \
		EXPECT_NEAR(w, (ew), 1E-7); \
	}

#endif /* TEST_UTILITIES_H */
