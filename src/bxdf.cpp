#include "../include/bxdf.h"

manta::BXDF::BXDF() {
    /* void */
}

manta::BXDF::~BXDF() {
    /* void */
}

void manta::BXDF::_evaluate() {
    setOutput(this);
}
