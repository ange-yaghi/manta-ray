#include <pch.h>

#include "utilities.h"

#include <perfect_specular_reflection_bsdf.h>
#include <lambertian_bsdf.h>
#include <bilayer_bsdf.h>
#include <stack_allocator.h>
#include <dielectric_media_interface.h>
#include <phong_distribution.h>
#include <microfacet_reflection_bsdf.h>

#include <manta_math.h>

using namespace manta;

TEST(BSDFTests, BSDFSanityCheck) {
	PerfectSpecularReflectionBSDF bsdf;

	math::Vector incident = math::loadVector((math::real)1.0, (math::real)1.0, (math::real)0.0);
	math::Vector outgoing;
	math::real pdf;

	math::Vector reflectance = bsdf.sampleF(nullptr, incident, &outgoing, &pdf, nullptr);

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
		math::Vector reflectance = bsdf.sampleF(nullptr, incident, &outgoing, &pdf, nullptr);

		math::Vector abs_cos_theta = math::abs(math::dot(outgoing, normal));
		accum = math::add(accum, math::div(math::mul(reflectance, abs_cos_theta), math::loadScalar(pdf))); //  * 1.33333
		//accum = math::add(accum, math::mul(reflectance, math::abs(math::dot(outgoing, normal))));
	}

	accum = math::div(accum, math::loadScalar((math::real)SAMPLE_COUNT));
	//accum = math::mul(accum, math::loadScalar(math::constants::PI));

	CHECK_VEC3_EQ(accum, math::constants::One, 1E-3);
}

TEST(BSDFTests, PhongBSDFEnergyConservation) {
	PhongDistribution dist;
	dist.setPower((math::real)5.0);

	MaterialNodeMemory mem;
	dist.initialize(nullptr, &mem, nullptr);

	MicrofacetReflectionBSDF bsdf;
	bsdf.setDistribution(&dist);

	math::Vector incident = math::loadVector((math::real)0.0, (math::real)0.0, (math::real)1.0);
	incident = math::normalize(incident);
	math::real pdf;
	math::Vector normal = math::loadVector((math::real)0.0, (math::real)0.0, (math::real)1.0);

	math::real accum = (math::real)0.0;
	constexpr int SAMPLE_COUNT = 100000;

	// Check that the distribution integrates to 1
	for (int i = 0; i < SAMPLE_COUNT; i++) {
		math::Vector m = dist.generateMicrosurfaceNormal(&mem);
		math::real cos_theta_h = math::getZ(m);
		math::real D = dist.calculateDistribution(m, &mem);
		math::real pdf = dist.calculatePDF(math::reflect(incident, m), m, &mem);

		if (pdf > 0.0) {
			accum += D * cos_theta_h / pdf;

			if (std::isnan(accum)) {
				assert(false);
			}
		}
	}

	accum = accum / (math::real)SAMPLE_COUNT;

	EXPECT_NEAR(accum, 1.0, 1E-4);
}

TEST(BSDFTests, BilayerBSDFEnergyConservation) {
	
	PhongDistribution phong;
	phong.setPower((math::real)4.0);

	MicrofacetReflectionBSDF bsdf1;
	bsdf1.setDistribution(&phong);
	//LambertianBSDF bsdf1;
	LambertianBSDF bsdf2;
	DielectricMediaInterface fresnel;
	fresnel.setIorIncident((math::real)1.0);
	fresnel.setIorTransmitted((math::real)1.5);

	BilayerBSDF bsdf;
	bsdf.setDiffuseMaterial(&bsdf2);
	bsdf.setCoatingDistribution(&phong);
	bsdf.setDiffuse(math::loadVector(1.0, 1.0, 1.0));
	bsdf.setSpecularAtNormal(math::loadVector(1.0, 1.0, 1.0));

	math::Vector incident = math::loadVector((math::real)0.0, (math::real)0.0, (math::real)1.0);
	incident = math::normalize(incident);
	math::real pdf;
	math::Vector normal = math::loadVector((math::real)0.0, (math::real)0.0, (math::real)1.0);

	math::Vector accum = math::constants::Zero;
	constexpr int SAMPLE_COUNT = 20000;

	StackAllocator s;
	s.initialize(1000);

	// Calculate rho
	for (int i = 0; i < SAMPLE_COUNT; i++) {
		math::Vector outgoing;
		math::Vector reflectance = bsdf.sampleF(nullptr, incident, &outgoing, &pdf, &s);

		if (pdf != 0.0) {
			math::Vector abs_cos_theta = math::abs(math::dot(outgoing, normal));
			accum = math::add(accum, math::div(math::mul(reflectance, abs_cos_theta), math::loadScalar(pdf))); //  * 1.33333
			//accum = math::add(accum, math::mul(reflectance, math::abs(math::dot(outgoing, normal))));
		}
	}

	accum = math::div(accum, math::loadScalar((math::real)SAMPLE_COUNT));
	//accum = math::mul(accum, math::loadScalar(math::constants::PI));

	EXPECT_LE(math::getX(accum), 1.0);
	EXPECT_LE(math::getY(accum), 1.0);
	EXPECT_LE(math::getZ(accum), 1.0);
}

