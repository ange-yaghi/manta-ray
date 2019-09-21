#include "../include/bxdf_to_bsdf_node.h"

manta::BxdfToBsdfNode::BxdfToBsdfNode() {
    /* void */
}

manta::BxdfToBsdfNode::~BxdfToBsdfNode() {
    /* void */
}

void manta::BxdfToBsdfNode::_initialize() {
    m_output.initialize();
}

void manta::BxdfToBsdfNode::_evaluate() {
    BXDF *bxdf = getObject<BXDF>(m_bxdfInput);

    m_bsdf.addBxdf(bxdf);
    m_output.setReference(&m_bsdf);
}

void manta::BxdfToBsdfNode::_destroy() {
    /* void */
}

void manta::BxdfToBsdfNode::registerInputs() {
    registerInput(&m_bxdfInput, "bxdf");
}
