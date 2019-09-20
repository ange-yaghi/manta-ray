#include "../include/bilayer_brdf.h"

#include "../include/microfacet_distribution.h"

#include <iostream>
#include <algorithm>
#include <assert.h>

manta::BilayerBRDF::BilayerBRDF() {
    m_diffuseNode = nullptr;
    m_specularNode = nullptr;

    m_diffuse = math::constants::One;
    m_specular = math::constants::One;
}

manta::BilayerBRDF::~BilayerBRDF() {
    /* void */
}

manta::math::Vector manta::BilayerBRDF::sampleF(const IntersectionPoint *surfaceInteraction, 
    const math::Vector &i, math::Vector *o, math::real *pdf, StackAllocator *stackAllocator) const 
{
    math::Vector diffuseR = m_diffuse;
    math::Vector specularR = m_specular;

    if (m_diffuseNode != nullptr) {
        math::Vector diffuse;
        VectorNodeOutput *diffuseNode = static_cast<VectorNodeOutput *>(m_diffuseNode);
        diffuseNode->sample(surfaceInteraction, (void *)&diffuse);
        diffuseR = math::mul(diffuse, diffuseR);
    }

    if (m_specularNode != nullptr) {
        math::Vector specular;
        VectorNodeOutput *specularNode = static_cast<VectorNodeOutput *>(m_specularNode);
        specularNode->sample(surfaceInteraction, (void *)&specular);
        specularR = math::mul(specular, specularR);
    }
    
    math::real u = math::uniformRandom();

    math::real coatingPDF;
    math::real diffusePDF;

    math::Vector wh;

    MicrofacetDistribution *distribution = 
        static_cast<ObjectReferenceNodeOutput<MicrofacetDistribution> *>(m_coatingDistribution)->getReference();

    math::Vector m;

    if (u < (math::real)0.5) {
        // Ray reflects off of the coating
        m = distribution->generateMicrosurfaceNormal(surfaceInteraction);
        math::Vector ri = math::reflect(i, m);
        wh = m;
        *o = ri;
    }
    else {
        // Ray is transmitted through the coating material
        math::Vector diffuseO;
        m_diffuseMaterial.sampleF(surfaceInteraction, i, &diffuseO, &diffusePDF, stackAllocator);
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
        coatingPDF = distribution->calculatePDF(wh, surfaceInteraction) / ((math::real)4 * o_dot_wh);
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
        specular = math::loadScalar(distribution->calculateDistribution(wh, surfaceInteraction));

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
    const math::Vector &i, const math::Vector &o, StackAllocator *stackAllocator) const 
{
    return math::constants::Zero;
}

manta::math::real manta::BilayerBRDF::pdf(
    const IntersectionPoint *surfaceInteraction, const math::Vector &i, const math::Vector &o) const 
{
    return math::real();
}

void manta::BilayerBRDF::registerInputs() {
    registerInput(&m_coatingDistribution, "coating");
    registerInput(&m_diffuseNode, "diffuse");
    registerInput(&m_specularNode, "specular");
}
