#include <pch.h>

#include "utilities.h"

#include "../include/perfect_specular_reflection_brdf.h"
#include "../include/lambertian_brdf.h"
#include "../include/bilayer_brdf.h"
#include "../include/stack_allocator.h"
#include "../include/dielectric_media_interface.h"
#include "../include/phong_distribution.h"
#include "../include/microfacet_brdf.h"
#include "../include/microfacet_btdf.h"
#include "../include/microfacet_glass_bsdf.h"
#include "../include/bsdf.h"
#include "../include/disney_diffuse_brdf.h"
#include "../include/disney_ggx_distribution.h"
#include "../include/disney_specular_brdf.h"
#include "../include/disney_gtr_clearcoat_distribution.h"
#include "../include/microfacet_brdf.h"

#include "../include/manta_math.h"

using namespace manta;

TEST(BSDFTests, BSDFSanityCheck) {
    PerfectSpecularReflectionBRDF brdf;

    math::Vector incident = math::loadVector((math::real)1.0, (math::real)1.0, (math::real)0.0);
    math::Vector outgoing;
    math::real pdf;
    RayFlags flags = RayFlag::None;

    math::Vector reflectance = brdf.sampleF(nullptr, math::Vector2(math::uniformRandom(), math::uniformRandom()), incident, &outgoing, &pdf, &flags, nullptr);

    math::Vector normal = math::loadVector((math::real)0.0, (math::real)0.0, (math::real)1.0);
    math::Vector expected = math::reflect(incident, normal);

    CHECK_VEC_EQ(outgoing, expected, 1E-6);
    EXPECT_NEAR(pdf, (math::real)1.0, 1E-6);
    CHECK_VEC_EQ(reflectance, math::constants::One, 1E-6);
}

TEST(BSDFTests, LambertBSDFEnergyConservation) {
    LambertianBRDF bsdf;

    math::Vector incident = math::loadVector((math::real)1.0, (math::real)1.0, (math::real)0.0);
    incident = math::normalize(incident);
    math::real pdf;
    math::Vector normal = math::loadVector((math::real)0.0, (math::real)0.0, (math::real)1.0);

    math::Vector accum = math::constants::Zero;
    constexpr int SAMPLE_COUNT = 100000;

    // Calculate rho
    for (int i = 0; i < SAMPLE_COUNT; i++) {
        math::Vector outgoing;
        RayFlags flags = RayFlag::None;
        math::Vector reflectance = bsdf.sampleF(nullptr, math::Vector2(math::uniformRandom(), math::uniformRandom()), incident, &outgoing, &pdf, &flags, nullptr);

        math::Vector abs_cos_theta = math::abs(math::dot(outgoing, normal));
        accum = math::add(accum, math::div(math::mul(reflectance, abs_cos_theta), math::loadScalar(pdf))); //  * 1.33333
        //accum = math::add(accum, math::mul(reflectance, math::abs(math::dot(outgoing, normal))));
    }

    accum = math::div(accum, math::loadScalar((math::real)SAMPLE_COUNT));
    //accum = math::mul(accum, math::loadScalar(math::constants::PI));

    CHECK_VEC3_EQ(accum, math::constants::One, 1E-3);
}

math::Vector comprehensiveBsdfTest(const math::Vector &incident, BSDF &bsdf, int sampleCount = 100000) {
    static __int64 testId = 0;

    math::Vector normal = math::loadVector((math::real)0.0, (math::real)0.0, (math::real)1.0);

    StackAllocator s;
    s.initialize(1000);

    IntersectionPoint point;
    point.m_direction = DielectricMediaInterface::Direction::In;

    math::Vector accum = math::constants::Zero;

    // Calculate rho
    for (int i = 0; i < sampleCount; i++) {
        // Non-functional code for debugging purposes
        if (testId == 105065) {
            int breakHere = 0;
        }

        math::real pdf;
        math::Vector outgoing;
        RayFlags flags = RayFlag::None;
        math::Vector reflectance = bsdf.sampleF(&point, math::Vector2(math::uniformRandom(), math::uniformRandom()), incident, &outgoing, &pdf, &flags, &s);

        EXPECT_VALID_VEC(reflectance);
        EXPECT_VALID(pdf);

        EXPECT_GE(pdf, 0.0f);

        if (pdf != 0.0f) {
            math::Vector abs_cos_theta = math::abs(math::dot(outgoing, normal));
            accum = math::add(accum, math::div(math::mul(reflectance, abs_cos_theta), math::loadScalar(pdf)));
        }

        testId++;
    }

    accum = math::div(accum, math::loadScalar((math::real)sampleCount));
    return accum;
}

TEST(BSDFTests, PhongMicrofacetEnergyConservation) {
    PhongDistribution dist;
    dist.setPower((math::real)5.0);

    MicrofacetBRDF bsdf;
    bsdf.setDistribution(&dist);

    math::Vector incident = math::loadVector((math::real)0.0, (math::real)0.0, (math::real)1.0);
    incident = math::normalize(incident);

    math::Vector normal = math::loadVector((math::real)0.0, (math::real)0.0, (math::real)1.0);

    math::real accum = (math::real)0.0;
    constexpr int SAMPLE_COUNT = 100000;

    // Check that the distribution integrates to 1
    for (int i = 0; i < SAMPLE_COUNT; i++) {
        math::Vector m = dist.generateMicrosurfaceNormal(nullptr, math::Vector2(math::uniformRandom(), math::uniformRandom()));
        math::real cos_theta_h = math::getZ(m);
        math::real D = dist.calculateDistribution(m, nullptr);
        math::real pdf = dist.calculatePDF(m, nullptr);

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

TEST(BSDFTests, BilayerBRDFEnergyConservation) {
    PhongDistribution phong;
    phong.setPower((math::real)4.0);

    MicrofacetBRDF bsdf1;
    bsdf1.setDistribution(&phong);
    //LambertianBRDF bsdf1;
    LambertianBRDF bsdf2;
    DielectricMediaInterface fresnel;
    fresnel.setIorIncident((math::real)1.0);
    fresnel.setIorTransmitted((math::real)1.5);

    BilayerBRDF brdf;
    brdf.setCoatingDistribution(&phong);
    brdf.setDiffuse(math::loadVector(1.0, 1.0, 1.0));
    brdf.setSpecularAtNormal(math::loadVector(1.0, 1.0, 1.0));

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
        RayFlags flags = RayFlag::None;
        math::Vector reflectance = brdf.sampleF(nullptr, math::Vector2(math::uniformRandom(), math::uniformRandom()), incident, &outgoing, &pdf, &flags, &s);

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

    MicrofacetBTDF btdf;
    btdf.setDistribution(&phong);
    //LambertianBRDF bsdf1;
    LambertianBRDF bsdf2;
    DielectricMediaInterface fresnel;
    fresnel.setIorIncident((math::real)1.0);
    fresnel.setIorTransmitted((math::real)1.5);
    btdf.setMediaInterface(&fresnel);

    math::Vector incident = math::loadVector((math::real)1.0, (math::real)0.0, (math::real)1.0);
    incident = math::normalize(incident);
    math::real pdf;
    math::Vector normal = math::loadVector((math::real)0.0, (math::real)0.0, (math::real)1.0);

    math::Vector accum = math::constants::Zero;
    constexpr int SAMPLE_COUNT = 20000;

    StackAllocator s;
    s.initialize(1000);

    IntersectionPoint point;
    point.m_direction = DielectricMediaInterface::Direction::In;

    // Calculate rho
    for (int i = 0; i < SAMPLE_COUNT; i++) {
        math::Vector outgoing;
        RayFlags flags = RayFlag::None;
        math::Vector transmitance = btdf.sampleF(&point, math::Vector2(math::uniformRandom(), math::uniformRandom()), incident, &outgoing, &pdf, &flags, &s);

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
    point.m_direction = DielectricMediaInterface::Direction::Out;

    // Calculate rho
    for (int i = 0; i < SAMPLE_COUNT; i++) {
        math::Vector outgoing;
        RayFlags flags = RayFlag::None;
        math::Vector transmitance = bsdf.sampleF(&point, math::Vector2(math::uniformRandom(), math::uniformRandom()), incident, &outgoing, &pdf, &flags, &s);

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

TEST(BSDFTests, CompoundBSDFNanTest) {
    // Generate Disney BSDF
    DisneyDiffuseBRDF diffuseBrdf;
    diffuseBrdf.setBaseColor(math::constants::One);
    diffuseBrdf.setRoughness((math::real)0.5f);

    DisneyGgxDistribution ggx;
    ggx.setRoughness((math::real)0.5f);

    DisneySpecularBRDF specularBrdf;
    specularBrdf.setBaseColor(math::constants::One);
    specularBrdf.setRoughness((math::real)0.5f);
    specularBrdf.setSpecular((math::real)0.04f);
    specularBrdf.setDistribution(&ggx);

    BSDF disneyBsdf;
    disneyBsdf.addBxdf(&diffuseBrdf);
    disneyBsdf.addBxdf(&specularBrdf);

    math::Vector incident = math::loadVector((math::real)0.0, (math::real)0.0, (math::real)1.0);
    incident = math::normalize(incident);
    math::real pdf;
    math::Vector normal = math::loadVector((math::real)0.0, (math::real)0.0, (math::real)1.0);

    math::Vector accum = math::constants::Zero;
    constexpr int SAMPLE_COUNT = 20000000;

    StackAllocator s;
    s.initialize(1000);

    IntersectionPoint point;
    point.m_direction = DielectricMediaInterface::Direction::In;

    // Calculate rho
    for (int i = 0; i < SAMPLE_COUNT; i++) {
        // Non-functional code for debugging purposes
        if (i == -1) {
            int breakHere = 0;
        }

        math::Vector outgoing;
        RayFlags flags = RayFlag::None;
        math::Vector reflectance = disneyBsdf.sampleF(&point, math::Vector2(math::uniformRandom(), math::uniformRandom()), incident, &outgoing, &pdf, &flags, &s);

        EXPECT_GE(math::getX(reflectance), 0.0f);
        EXPECT_GE(math::getY(reflectance), 0.0f);
        EXPECT_GE(math::getZ(reflectance), 0.0f);

        EXPECT_FALSE(std::isnan(math::getX(reflectance)));
        EXPECT_FALSE(std::isnan(math::getY(reflectance)));
        EXPECT_FALSE(std::isnan(math::getZ(reflectance)));

        EXPECT_FALSE(std::isinf(math::getX(reflectance)));
        EXPECT_FALSE(std::isinf(math::getY(reflectance)));
        EXPECT_FALSE(std::isinf(math::getZ(reflectance)));

        EXPECT_FALSE(std::isnan(pdf));

        EXPECT_FALSE(std::isnan(math::getX(reflectance)));
        EXPECT_FALSE(std::isnan(math::getY(reflectance)));
        EXPECT_FALSE(std::isnan(math::getZ(reflectance)));

        EXPECT_GE(pdf, 0.0f);

        if (pdf != 0.0f) {
            math::Vector abs_cos_theta = math::abs(math::dot(outgoing, normal));
            accum = math::add(accum, math::div(math::mul(reflectance, abs_cos_theta), math::loadScalar(pdf)));
        }
    }

    accum = math::div(accum, math::loadScalar((math::real)SAMPLE_COUNT));

    EXPECT_LE(math::getX(accum), 1.1f);
    EXPECT_LE(math::getY(accum), 1.1f);
    EXPECT_LE(math::getZ(accum), 1.1f);
}

TEST(BSDFTests, DisneyClearCoatTest) {
    // Generate Disney Clearcoat BSDF
    DisneyGtrClearcoatDistribution dist;
    dist.setRoughness(0.1f);

    MicrofacetBRDF brdf;
    brdf.setDistribution(&dist);
    brdf.setBaseReflectivity(math::constants::One);

    math::Vector incident = math::loadVector((math::real)0.0, (math::real)0.0, (math::real)1.0);
    incident = math::normalize(incident);
    math::real pdf;
    math::Vector normal = math::loadVector((math::real)0.0, (math::real)0.0, (math::real)1.0);

    math::Vector accum = math::constants::Zero;
    constexpr int SAMPLE_COUNT = 20000000;

    StackAllocator s;
    s.initialize(1000);

    IntersectionPoint point;
    point.m_direction = DielectricMediaInterface::Direction::In;

    // Calculate rho
    for (int i = 0; i < SAMPLE_COUNT; i++) {
        // Non-functional code for debugging purposes
        if (i == -1) {
            int breakHere = 0;
        }

        math::Vector outgoing;
        RayFlags flags = RayFlag::None;
        math::Vector reflectance = brdf.sampleF(&point, math::Vector2(math::uniformRandom(), math::uniformRandom()), incident, &outgoing, &pdf, &flags, &s);

        EXPECT_GE(math::getX(reflectance), 0.0f);
        EXPECT_GE(math::getY(reflectance), 0.0f);
        EXPECT_GE(math::getZ(reflectance), 0.0f);

        EXPECT_FALSE(std::isnan(math::getX(reflectance)));
        EXPECT_FALSE(std::isnan(math::getY(reflectance)));
        EXPECT_FALSE(std::isnan(math::getZ(reflectance)));

        EXPECT_FALSE(std::isinf(math::getX(reflectance)));
        EXPECT_FALSE(std::isinf(math::getY(reflectance)));
        EXPECT_FALSE(std::isinf(math::getZ(reflectance)));

        EXPECT_FALSE(std::isnan(pdf));

        EXPECT_FALSE(std::isnan(math::getX(reflectance)));
        EXPECT_FALSE(std::isnan(math::getY(reflectance)));
        EXPECT_FALSE(std::isnan(math::getZ(reflectance)));

        EXPECT_GE(pdf, 0.0f);

        if (pdf != 0.0f) {
            math::Vector abs_cos_theta = math::abs(math::dot(outgoing, normal));
            accum = math::add(accum, math::div(math::mul(reflectance, abs_cos_theta), math::loadScalar(pdf)));
        }
    }

    accum = math::div(accum, math::loadScalar((math::real)SAMPLE_COUNT));

    EXPECT_LE(math::getX(accum), 1.1f);
    EXPECT_LE(math::getY(accum), 1.1f);
    EXPECT_LE(math::getZ(accum), 1.1f);
}

TEST(BSDFTests, DisneyEdgeCaseTest) {
    // Generate Disney BSDF
    DisneyDiffuseBRDF diffuseBrdf;
    diffuseBrdf.setBaseColor(math::constants::One);
    diffuseBrdf.setRoughness((math::real)0.5f);

    DisneyGgxDistribution ggx;
    ggx.setRoughness((math::real)0.5f);

    DisneySpecularBRDF specularBrdf;
    specularBrdf.setBaseColor(math::constants::One);
    specularBrdf.setRoughness((math::real)0.5f);
    specularBrdf.setSpecular((math::real)0.5f);
    specularBrdf.setDistribution(&ggx);

    BSDF disneyBsdf;
    disneyBsdf.addBxdf(&diffuseBrdf);
    disneyBsdf.addBxdf(&specularBrdf);

    math::Vector accum = math::constants::Zero;
    accum = comprehensiveBsdfTest(math::loadVector(0.0f, 0.0f, 1.0f), disneyBsdf);
    EXPECT_VALID_VEC(accum);

    diffuseBrdf.setRoughness(0.0f);
    specularBrdf.setRoughness(0.0f);
    ggx.setRoughness(0.0f);
    accum = comprehensiveBsdfTest(math::normalize(math::loadVector(0.0f, 1.0f, 1.0f)), disneyBsdf);
    EXPECT_VALID_VEC(accum);

    diffuseBrdf.setRoughness(0.01f);
    specularBrdf.setRoughness(0.01f);
    accum = comprehensiveBsdfTest(math::loadVector(0.0f, 0.0f, 1.0f), disneyBsdf);
    EXPECT_VALID_VEC(accum);
}
