#include <pch.h>

#include "utilities.h"

#include <perfect_specular_reflection_bsdf.h>
#include <lambertian_bsdf.h>

#include <manta_math.h>

using namespace manta;

TEST(BSDFTests, BSDFSanityCheck) {
	PerfectSpecularReflectionBSDF bsdf;

	math::Vector incident = math::loadVector((math::real)1.0, (math::real)1.0, (math::real)0.0);
	math::Vector outgoing;
	math::real pdf;

	math::Vector reflectance = bsdf.sampleF(incident, &outgoing, &pdf);

	math::Vector normal = math::loadVector((math::real)0.0, (math::real)0.0, (math::real)1.0);
	math::Vector expected = math::reflect(incident, normal);

	CHECK_VEC_EQ(outgoing, expected, 1E-6);
	EXPECT_NEAR(pdf, (math::real)1.0, 1E-6);
	CHECK_VEC_EQ(reflectance, math::constants::One, 1E-6);
}

TEST(BSDFTests, LambertBSDFEnergyConservation) {
	LambertianBSDF bsdf;

	math::Vector incident = math::loadVector((math::real)1.0, (math::real)1.0, (math::real)0.0);
	incident = math::normalize(incident);
	math::real pdf;
	math::Vector normal = math::loadVector((math::real)0.0, (math::real)0.0, (math::real)1.0);

	math::Vector accum = math::constants::Zero;
	constexpr int SAMPLE_COUNT = 100000;

	// Calculate rho
	for (int i = 0; i < SAMPLE_COUNT; i++) {
		math::Vector outgoing;
		math::Vector reflectance = bsdf.sampleF(incident, &outgoing, &pdf);

		math::Vector abs_cos_theta = math::abs(math::dot(outgoing, normal));
		accum = math::add(accum, math::div(math::mul(reflectance, abs_cos_theta), math::loadScalar(pdf))); //  * 1.33333
		//accum = math::add(accum, math::mul(reflectance, math::abs(math::dot(outgoing, normal))));
	}

	accum = math::div(accum, math::loadScalar((math::real)SAMPLE_COUNT));
	//accum = math::mul(accum, math::loadScalar(math::constants::PI));

	CHECK_VEC3_EQ(accum, math::constants::One, 1E-3);
}
