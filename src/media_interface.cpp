#include "../include/media_interface.h"

manta::MediaInterface::MediaInterface() {
    /* void */
}

manta::MediaInterface::~MediaInterface() {
    /* void */
}

void manta::MediaInterface::_evaluate() {
    m_output.setReference(this);
}
