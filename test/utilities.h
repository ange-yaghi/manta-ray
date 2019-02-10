#ifndef TEST_UTILITIES_H
#define TEST_UTILITIES_H

#define CHECK_VEC(v, ex, ey, ez, ew) {\
		math::real x = math::getX((v)); \
		math::real y = math::getY((v)); \
		math::real z = math::getZ((v)); \
		math::real w = math::getW((v)); \
		EXPECT_NEAR(x, (ex), 1E-7); \
		EXPECT_NEAR(y, (ey), 1E-7); \
		EXPECT_NEAR(z, (ez), 1E-7); \
		EXPECT_NEAR(w, (ew), 1E-7); \
	}

#define CHECK_VEC_EQ(a, b, eps) {\
		math::real xa = math::getX((a)); \
		math::real ya = math::getY((a)); \
		math::real za = math::getZ((a)); \
		math::real wa = math::getW((a)); \
		math::real xb = math::getX((b)); \
		math::real yb = math::getY((b)); \
		math::real zb = math::getZ((b)); \
		math::real wb = math::getW((b)); \
		EXPECT_NEAR(xa, (xb), eps); \
		EXPECT_NEAR(ya, (yb), eps); \
		EXPECT_NEAR(za, (zb), eps); \
		EXPECT_NEAR(wa, (wb), eps); \
	}

#define CHECK_VEC3_EQ(a, b, eps) {\
		math::real xa = math::getX((a)); \
		math::real ya = math::getY((a)); \
		math::real za = math::getZ((a)); \
		math::real xb = math::getX((b)); \
		math::real yb = math::getY((b)); \
		math::real zb = math::getZ((b)); \
		EXPECT_NEAR(xa, (xb), eps); \
		EXPECT_NEAR(ya, (yb), eps); \
		EXPECT_NEAR(za, (zb), eps); \
	}

#define CHECK_QUAT(v, ew, ex, ey, ez) {\
		math::real x = math::getQuatX((v)); \
		math::real y = math::getQuatY((v)); \
		math::real z = math::getQuatZ((v)); \
		math::real w = math::getQuatW((v)); \
		EXPECT_NEAR(x, (ex), 1E-7); \
		EXPECT_NEAR(y, (ey), 1E-7); \
		EXPECT_NEAR(z, (ez), 1E-7); \
		EXPECT_NEAR(w, (ew), 1E-7); \
	}

#define CHECK_QUAT_EQ(v, ev) {\
		math::real x = math::getQuatX((v)); \
		math::real y = math::getQuatY((v)); \
		math::real z = math::getQuatZ((v)); \
		math::real w = math::getQuatW((v)); \
		math::real ex = math::getQuatX((ev)); \
		math::real ey = math::getQuatY((ev)); \
		math::real ez = math::getQuatZ((ev)); \
		math::real ew = math::getQuatW((ev)); \
		EXPECT_NEAR(x, (ex), 1E-7); \
		EXPECT_NEAR(y, (ey), 1E-7); \
		EXPECT_NEAR(z, (ez), 1E-7); \
		EXPECT_NEAR(w, (ew), 1E-7); \
	}

#endif /* TEST_UTILITIES_H */
