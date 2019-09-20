#include "../include/simple_bsdf_material.h"

#include "../include/ray_container.h"
#include "../include/intersection_point.h"
#include "../include/light_ray.h"
#include "../include/vector_node_output.h"
#include "../include/bsdf.h"
#include "../include/lambertian_brdf.h"

manta::SimpleBSDFMaterial::SimpleBSDFMaterial() {
    m_maxDegree = 8;

    m_emissionNode = nullptr;
    m_reflectanceNode = nullptr;

    m_bsdf = nullptr;
    m_defaultBsdf = nullptr;

    m_reflectance = math::constants::One;
    m_emission = math::constants::One;
}

manta::SimpleBSDFMaterial::~SimpleBSDFMaterial() {
    /* void */
}

const manta::VectorNodeOutput *manta::SimpleBSDFMaterial::getReflectanceNode() const {
    return static_cast<manta::VectorNodeOutput *>(m_reflectanceNode);
}

const manta::VectorNodeOutput *manta::SimpleBSDFMaterial::getEmissionNode() const {
    return static_cast<manta::VectorNodeOutput *>(m_emissionNode);
}

manta::math::Vector manta::SimpleBSDFMaterial::getFilterColor(const IntersectionPoint &ip) const {
    math::Vector reflectance = m_reflectance;
    if (m_reflectanceNode != nullptr) {
        math::Vector r;
        getReflectanceNode()->sample(&ip, (void *)&r);

        reflectance = math::mul(reflectance, r);
    }

    return reflectance;
}

manta::math::Vector manta::SimpleBSDFMaterial::getEmission(const IntersectionPoint &ip) const {
    math::Vector emission = m_emission;
    if (m_emissionNode != nullptr) {
        math::Vector e;
        getEmissionNode()->sample(&ip, (void *)&e);

        emission = math::mul(emission, e);
    }

    return emission;
}

void manta::SimpleBSDFMaterial::_evaluate() {
    Material::_evaluate();

    m_bsdf = getObject<BSDF>(m_bsdfInput);
}

void manta::SimpleBSDFMaterial::_initialize() {
    /* void */
}

void manta::SimpleBSDFMaterial::registerInputs() {
    Material::registerInputs();
    registerInput(&m_reflectanceNode, "reflectance");
    registerInput(&m_emissionNode, "emission");
    registerInput(&m_bsdfInput, "bsdf");
}
