#include "../include/bilayer_brdf.h"

#include "../include/microfacet_distribution.h"

#include <iostream>
#include <algorithm>
#include <assert.h>

manta::BilayerBRDF::BilayerBRDF() {
    m_coatingDistributionNode = nullptr;

    m_diffuse.setDefault(math::constants::One);
    m_specular.setDefault(math::constants::One);
}

manta::BilayerBRDF::~BilayerBRDF() {
    /* void */
}

manta::math::Vector manta::BilayerBRDF::sampleF(const IntersectionPoint *surfaceInteraction, 
    const math::Vector2 &u, const math::Vector &i, math::Vector *o, math::real *pdf, StackAllocator *stackAllocator)
{
    math::Vector diffuseR = m_diffuse.sample(surfaceInteraction);
    math::Vector specularR = m_specular.sample(surfaceInteraction);

    math::real d = u.x;

    math::real coatingPDF;
    math::real diffusePDF;

    math::Vector wh;

    if (d < (math::real)0.5) {
        // Ray reflects off of the coating
        math::Vector2 nu = math::Vector2(d * (math::real)2.0, u.y);
        wh = m_coatingDistribution->generateMicrosurfaceNormal(surfaceInteraction, nu);
        math::Vector ri = math::reflect(i, wh);

        *o = ri;
    }
    else {
        // Ray is transmitted through the coating material
        math::Vector2 nu = math::Vector2((d - (math::real)0.5) * (math::real)2.0, u.y);
        math::Vector diffuseO;

        // Uniformly sample a hemisphere
        math::real r1 = nu.x * math::constants::TWO_PI;
        math::real r2 = nu.y;
        math::real r2s = (math::real)sqrt(1 - r2 * r2);

        math::Vector direction = math::loadVector(
            cos(r1) * r2s,
            sin(r1) * r2s,
            r2);

        diffuseO = direction;
        diffusePDF = (math::real)1.0 / math::constants::TWO_PI;

        wh = math::add(diffuseO, i);

        *o = diffuseO;
    }
    
    if (math::getX(wh) == 0 && math::getY(wh) == 0 && math::getZ(wh) == 0) {
        *pdf = 0.0;
        return math::constants::Zero;
    }

    wh = math::normalize(wh);

    math::real cosThetaO = math::getZ(*o);
    math::real cosThetaI = math::getZ(i);

    if (cosThetaO <= (math::real)0.0 || cosThetaI <= (math::real)0.0) {
        *pdf = 0.0;
        return math::constants::Zero;
    }

    math::real o_dot_wh;
    math::real abs_o_dot_wh;
    o_dot_wh = math::getScalar(math::dot(wh, *o));
    abs_o_dot_wh = ::abs(o_dot_wh);

    if (o_dot_wh <= 0 || math::getZ(wh) < 0) coatingPDF = 0.0;
    else {
        coatingPDF = m_coatingDistribution->calculatePDF(wh, surfaceInteraction) / ((math::real)4 * o_dot_wh);
    }

    diffusePDF = (math::real)1.0 / math::constants::TWO_PI;

    *pdf = (math::real)0.5 * (coatingPDF + diffusePDF);

    // Calculate reflectance
    auto pow5 = [](math::real v) { return (v * v) * (v * v) * v; };

    math::real absCosThetaI = ::abs(cosThetaI);
    math::real absCosThetaO = ::abs(cosThetaO);

    math::Vector diffuse = math::loadScalar((math::real)28.0 / ((math::real)23.0 * math::constants::PI));
    diffuse = math::mul(diffuse, diffuseR);
    diffuse = math::mul(diffuse, math::sub(math::constants::One, specularR));
    diffuse = math::mul(diffuse, math::loadScalar(1 - pow5(1 - (math::real)0.5 * absCosThetaI)));
    diffuse = math::mul(diffuse, math::loadScalar(1 - pow5(1 - (math::real)0.5 * absCosThetaO)));

    math::Vector specular;
    
    math::real absCosThetaOI = std::max(absCosThetaI, absCosThetaO);
    if (o_dot_wh <= 0 || absCosThetaOI == 0 || math::getZ(wh) < 0) {
        specular = math::constants::Zero;
    }
    else {
        specular = math::loadScalar(m_coatingDistribution->calculateDistribution(wh, surfaceInteraction));

        math::Vector specular_div = math::loadScalar(4 * abs_o_dot_wh * absCosThetaOI);
        math::Vector schlickFresnel = math::sub(math::constants::One, specularR);
        schlickFresnel = math::mul(schlickFresnel, math::loadScalar(pow5(1 - o_dot_wh)));
        schlickFresnel = math::add(schlickFresnel, specularR);

        specular = math::div(specular, specular_div);
        specular = math::mul(specular, schlickFresnel);
    }

    // Return reflectance
    math::Vector fr = math::add(diffuse, specular);

    assert(!(::isnan(math::getX(fr)) || ::isnan(math::getY(fr)) || ::isnan(math::getZ(fr))));

    return fr;
}

manta::math::Vector manta::BilayerBRDF::f(const IntersectionPoint *surfaceInteraction, 
    const math::Vector &i, const math::Vector &o, StackAllocator *stackAllocator) 
{
    return math::constants::Zero;
}

manta::math::real manta::BilayerBRDF::pdf(
    const IntersectionPoint *surfaceInteraction, const math::Vector &i, const math::Vector &o) 
{
    return math::real();
}

void manta::BilayerBRDF::setCoatingDistribution(MicrofacetDistribution *distribution) {
    m_coatingDistribution = distribution;
}

void manta::BilayerBRDF::registerInputs() {
    registerInput(&m_coatingDistributionNode, "coating");
    registerInput(m_diffuse.getPortAddress(), "diffuse");
    registerInput(m_specular.getPortAddress(), "specular");
}

void manta::BilayerBRDF::_evaluate() {
    BXDF::_evaluate();

    if (m_coatingDistributionNode != nullptr) {
        m_coatingDistribution = getObject<MicrofacetDistribution>(m_coatingDistributionNode);
    }
}

piranha::Node *manta::BilayerBRDF::_optimize() {
    m_diffuse.optimize();
    m_specular.optimize();

    return this;
}
