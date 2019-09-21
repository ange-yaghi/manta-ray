#include "../include/sampler_2d.h"

manta::Sampler2d::Sampler2d() {
    /* void */
}

manta::Sampler2d::~Sampler2d() {
    /* void */
}

void manta::Sampler2d::_evaluate() {
    m_output.setReference(this);
}
