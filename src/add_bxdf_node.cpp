#include "../include/add_bxdf_node.h"

#include "../include/bsdf.h"
#include "../include/bxdf.h"

manta::AddBxdfNode::AddBxdfNode() {
    /* void */
}

manta::AddBxdfNode::~AddBxdfNode() {
    /* void */
}

void manta::AddBxdfNode::_initialize() {
    /* void */
}

void manta::AddBxdfNode::_evaluate() {
    BSDF *bsdf = getObject<BSDF>(m_bsdfInput);
    BXDF *bxdf = getObject<BXDF>(m_bxdfInput);

    if (bsdf == nullptr) return;
    if (bxdf == nullptr) return;

    bsdf->addBxdf(bxdf);
}

void manta::AddBxdfNode::_destroy() {
    /* void */
}

void manta::AddBxdfNode::registerInputs() {
    registerInput(&m_bsdfInput, "bsdf", true);
    registerInput(&m_bxdfInput, "bxdf", false);
}

void manta::AddBxdfNode::registerOutputs() {
    /* void */
}
