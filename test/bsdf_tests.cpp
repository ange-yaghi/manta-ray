#include <pch.h>

#include "utilities.h"

#include <perfect_specular_reflection_bsdf.h>
#include <lambertian_bsdf.h>
#include <bilayer_bsdf.h>
#include <stack_allocator.h>
#include <dielectric_media_interface.h>
#include <phong_distribution.h>
#include <microfacet_reflection_bsdf.h>
#include <microfacet_transmission_bsdf.h>
#include <microfacet_glass_bsdf.h>

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

TEST(BSDFTests, PhongMicrofacetEnergyConservation) {
	PhongDistribution dist;
	dist.setPower((math::real)5.0);

	Node::NodeSessionMemory mem;
	dist.initializeSessionMemory(nullptr, &mem, nullptr);

	MicrofacetReflectionBSDF bsdf;
	bsdf.setDistribution(&dist);

	math::Vector incident = math::loadVector((math::real)0.0, (math::real)0.0, (math::real)1.0);
	incident = math::normalize(incident);

	math::Vector normal = math::loadVector((math::real)0.0, (math::real)0.0, (math::real)1.0);

	math::real accum = (math::real)0.0;
	constexpr int SAMPLE_COUNT = 100000;

	// Check that the distribution integrates to 1
	for (int i = 0; i < SAMPLE_COUNT; i++) {
		math::Vector m = dist.generateMicrosurfaceNormal(&mem);
		math::real cos_theta_h = math::getZ(m);
		math::real D = dist.calculateDistribution(m, &mem);
		math::real pdf = dist.calculatePDF(m, &mem);

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
	bsdf.setCoatingDistribution(phong.getMainOutput());
	bsdf.setDiffuse(math::loadVector(1.0, 1.0, 1.0));
	bsdf.setSpecularAtNormal(math::loadVector(1.0, 1.0, 1.0));

	math::Vector incident = math::loadVector((math::real)1000.0, (math::real)0.0, (math::real)1.0);
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

		EXPECT_GE(math::getX(reflectance), 0.0);
		EXPECT_GE(math::getY(reflectance), 0.0);
		EXPECT_GE(math::getZ(reflectance), 0.0);

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

TEST(BSDFTests, TransmissionBSDFTest) {
	PhongDistribution phong;
	phong.setPower((math::real)4.0);

	MicrofacetTransmissionBSDF bsdf;
	bsdf.setDistribution(&phong);
	//LambertianBSDF bsdf1;
	LambertianBSDF bsdf2;
	DielectricMediaInterface fresnel;
	fresnel.setIorIncident((math::real)1.0);
	fresnel.setIorTransmitted((math::real)1.5);
	bsdf.setMediaInterface(&fresnel);

	math::Vector incident = math::loadVector((math::real)1.0, (math::real)0.0, (math::real)1.0);
	incident = math::normalize(incident);
	math::real pdf;
	math::Vector normal = math::loadVector((math::real)0.0, (math::real)0.0, (math::real)1.0);

	math::Vector accum = math::constants::Zero;
	constexpr int SAMPLE_COUNT = 20000;

	StackAllocator s;
	s.initialize(1000);

	IntersectionPoint point;
	point.m_direction = DielectricMediaInterface::DIRECTION_IN;

	// Calculate rho
	for (int i = 0; i < SAMPLE_COUNT; i++) {
		math::Vector outgoing;
		math::Vector transmitance = bsdf.sampleF(&point, incident, &outgoing, &pdf, &s);

		EXPECT_LT(math::getZ(outgoing), 0.0);

		EXPECT_GE(pdf, 0.0);
		EXPECT_GE(math::getX(transmitance), 0.0);
		EXPECT_GE(math::getY(transmitance), 0.0);
		EXPECT_GE(math::getZ(transmitance), 0.0);

		if (pdf != 0.0) {
			math::Vector abs_cos_theta = math::abs(math::dot(outgoing, normal));
			accum = math::add(accum, math::div(math::mul(transmitance, abs_cos_theta), math::loadScalar(pdf))); //  * 1.33333
			//accum = math::add(accum, math::mul(reflectance, math::abs(math::dot(outgoing, normal))));
		}
	}

	accum = math::div(accum, math::loadScalar((math::real)SAMPLE_COUNT));
	//accum = math::mul(accum, math::loadScalar(math::constants::PI));

	EXPECT_LE(math::getX(accum), 1.0);
	EXPECT_LE(math::getY(accum), 1.0);
	EXPECT_LE(math::getZ(accum), 1.0);
}

TEST(BSDFTests, RefractionTest) {
	math::Vector n = math::loadVector(0.f, 1.f, 0.f);
	n = math::normalize(n);

	math::Vector v = math::loadVector(0.f, 1.f, 0.8f);
	v = math::normalize(v);

	math::Vector i;
	BSDF::refract(v, n, 1.5f, &i);

	math::real mag = math::getScalar(math::magnitude(i));

	EXPECT_NEAR(mag, 1.0f, 1E-5f);
}

TEST(BSDFTests, GlassBSDFNANTest) {
	PhongDistribution phong;
	phong.setPower((math::real)1.0);

	MicrofacetGlassBSDF bsdf;
	bsdf.setDistribution(&phong);
	DielectricMediaInterface fresnel;
	fresnel.setIorIncident((math::real)1.0);
	fresnel.setIorTransmitted((math::real)1.5);
	bsdf.setMediaInterface(&fresnel);

	math::Vector incident = math::loadVector((math::real)1.0, (math::real)0.0, (math::real)0.0);
	incident = math::normalize(incident);
	math::real pdf;
	math::Vector normal = math::loadVector((math::real)0.0, (math::real)0.0, (math::real)1.0);

	math::Vector accum = math::constants::Zero;
	constexpr int SAMPLE_COUNT = 2000000;

	StackAllocator s;
	s.initialize(1000);

	IntersectionPoint point;
	point.m_direction = DielectricMediaInterface::DIRECTION_OUT;

	// Calculate rho
	for (int i = 0; i < SAMPLE_COUNT; i++) {
		math::Vector outgoing;
		math::Vector transmitance = bsdf.sampleF(&point, incident, &outgoing, &pdf, &s);

		EXPECT_GE(math::getX(transmitance), 0.0f);
		EXPECT_GE(math::getY(transmitance), 0.0f);
		EXPECT_GE(math::getZ(transmitance), 0.0f);

		EXPECT_FALSE(std::isnan(math::getX(transmitance)));
		EXPECT_FALSE(std::isnan(math::getY(transmitance)));
		EXPECT_FALSE(std::isnan(math::getZ(transmitance)));

		EXPECT_FALSE(std::isinf(math::getX(transmitance)));
		EXPECT_FALSE(std::isinf(math::getY(transmitance)));
		EXPECT_FALSE(std::isinf(math::getZ(transmitance)));

		EXPECT_FALSE(std::isnan(pdf));

		EXPECT_FALSE(std::isnan(math::getX(outgoing)));
		EXPECT_FALSE(std::isnan(math::getY(outgoing)));
		EXPECT_FALSE(std::isnan(math::getZ(outgoing)));

		EXPECT_GE(pdf, 0.0f);

		if (pdf != 0.0f) {
			math::Vector abs_cos_theta = math::abs(math::dot(outgoing, normal));
			accum = math::add(accum, math::div(math::mul(transmitance, abs_cos_theta), math::loadScalar(pdf))); //  * 1.33333
			//accum = math::add(accum, math::mul(reflectance, math::abs(math::dot(outgoing, normal))));
		}
	}

	accum = math::div(accum, math::loadScalar((math::real)SAMPLE_COUNT));
	//accum = math::mul(accum, math::loadScalar(math::constants::PI));

	EXPECT_LE(math::getX(accum), 1.0f);
	EXPECT_LE(math::getY(accum), 1.0f);
	EXPECT_LE(math::getZ(accum), 1.0f);
}
