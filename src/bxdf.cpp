#include "../include/bxdf.h"

manta::BXDF::BXDF() {
    /* void */
}

manta::BXDF::~BXDF() {
    /* void */
}

void manta::BXDF::_evaluate() {
    m_output.setReference(this);
}
