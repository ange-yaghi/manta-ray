#include "../include/simple_bsdf_material.h"

#include "../include/intersection_point.h"
#include "../include/light_ray.h"
#include "../include/vector_node_output.h"
#include "../include/bsdf.h"
#include "../include/lambertian_brdf.h"

manta::SimpleBSDFMaterial::SimpleBSDFMaterial() {
    m_maxDegree = 8;

    m_bsdf = nullptr;
    m_defaultBsdf = nullptr;

    m_reflectance.setDefault(math::constants::One);
    m_emission.setDefault(math::constants::One);
}

manta::SimpleBSDFMaterial::~SimpleBSDFMaterial() {
    /* void */
}

const manta::VectorNodeOutput *manta::SimpleBSDFMaterial::getReflectanceNode() const {
    return static_cast<manta::VectorNodeOutput *>(m_reflectance.getPort());
}

const manta::VectorNodeOutput *manta::SimpleBSDFMaterial::getEmissionNode() const {
    return static_cast<manta::VectorNodeOutput *>(m_emission.getPort());
}

manta::math::Vector manta::SimpleBSDFMaterial::getFilterColor(const IntersectionPoint &ip) {
    return m_reflectance.sample(&ip);
}

manta::math::Vector manta::SimpleBSDFMaterial::getEmission(const IntersectionPoint &ip) {
    return m_emission.sample(&ip);
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
    registerInput(m_reflectance.getPortAddress(), "reflectance");
    registerInput(m_emission.getPortAddress(), "emission");
    registerInput(&m_bsdfInput, "bsdf");
}
