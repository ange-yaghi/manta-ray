#include <pch.h>
#include "utilities.h"

#include <manta_math.h>

namespace math = manta::math;

TEST(MathTests, VectorSanityCheck) {
	math::Vector v;
	v = math::loadVector(1.0f, 2.0f, 3.0f, 4.0f);
	CHECK_VEC(v, 1.0f, 2.0f, 3.0f, 4.0f);
}

TEST(MathTests, VectorLoadScalar) {
	math::Vector v;
	v = math::loadScalar(666.0f);
	CHECK_VEC(v, 666.0f, 666.0f, 666.0f, 666.0f);
}

TEST(MathTests, VectorLoadVector) {
	math::Vector v;
	v = math::loadVector(math::Vector4(100.0f, 200.0f, 300.0f, 400.0f));
	CHECK_VEC(v, 100.0f, 200.0f, 300.0f, 400.0f);
}

TEST(MathTests, VectorLoadVector3) {
	math::Vector v;
	v = math::loadVector(math::Vector3(100.0f, 200.0f, 300.0f));
	CHECK_VEC(v, 100.0f, 200.0f, 300.0f, 0.0f);
}

TEST(MathTests, VectorLoadVector2) {
	math::Vector v;
	v = math::loadVector(math::Vector2(100.0f, 200.0f));
	CHECK_VEC(v, 100.0f, 200.0f, 0.0f, 0.0f);
}

TEST(MathTests, VectorLoadVector2x2) {
	math::Vector v;
	v = math::loadVector(math::Vector2(100.0f, 200.0f), math::Vector2(300.0f, 400.0f));
	CHECK_VEC(v, 100.0f, 200.0f, 300.0f, 400.0f);
}

TEST(MathTests, QuaternionLoad) {
	math::Vector axis = math::loadVector(1.0f, 1.0f, 1.0f, 0.0f);
	axis = math::normalize(axis);

	math::real angle = 2.0f * math::constants::PI / 3.0f;
	math::Quaternion quat = math::loadQuaternion(angle, axis);
	math::Quaternion inv = math::quatInvert(quat);

	CHECK_QUAT(quat, 0.5f, 0.5f, 0.5f, 0.5f);
	CHECK_QUAT(inv, 0.5f, -0.5f, -0.5f, -0.5f);
}

TEST(MathTests, QuaternionMultiply) {
	math::Vector axis = math::loadVector(1.0f, 1.0f, 1.0f, 0.0f);
	axis = math::normalize(axis);

	math::real angle = math::constants::PI / 2.0f;
	math::Quaternion quat1 = math::loadQuaternion(angle, axis);

	math::real sumAngle = angle*2;
	math::Quaternion sumQuat = math::loadQuaternion(sumAngle, axis);

	math::Quaternion mulQuat = math::quatMultiply(quat1, quat1);

	CHECK_QUAT_EQ(mulQuat, sumQuat);
}

TEST(MathTests, QuaternionTransformation) {
	math::Vector axis = math::loadVector(1.0f, 1.0f, 1.0f, 0.0f);
	axis = math::normalize(axis);

	math::real angle = 2.0f * math::constants::PI / 3.0f;
	math::Quaternion quat = math::loadQuaternion(angle, axis);
	math::Quaternion inv = math::quatInvert(quat);

	CHECK_QUAT(quat, 0.5f, 0.5f, 0.5f, 0.5f);
	CHECK_QUAT(inv, 0.5f , -0.5f, -0.5f, -0.5f);

	math::Vector point = math::loadVector(1.0f, 0.0f, 0.0f, 0.0f);
	math::Vector transformed = math::quatTransform(quat, point);

	CHECK_VEC(transformed, 0.0f, 1.0f, 0.0f, 0.0f);
}

TEST(MathTests, QuaternionIdentityTest) {
	math::Vector axis = math::loadVector(1.0f, 1.0f, 1.0f, 0.0f);
	axis = math::normalize(axis);

	math::real angle = 2.0f * math::constants::PI / 3.0f;
	math::Quaternion quat = math::loadQuaternion(angle, axis);
	math::Quaternion inv = math::quatInvert(quat);

	math::Quaternion id = math::quatMultiply(quat, inv);
	CHECK_QUAT(id, 1.0f, 0.0f, 0.0f, 0.0f);
}

TEST(MathTests, PermutationTest) {
	math::Vector v = math::loadVector((math::real)1.0, (math::real)2.0, (math::real)3.0, (math::real)4.0);
	v = math::permute(v, 1, 0, 2, 3);
	CHECK_VEC(v, 2.0, 1.0, 3.0, 4.0);
}
