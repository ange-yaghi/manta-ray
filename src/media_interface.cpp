#include "../include/media_interface.h"

manta::MediaInterface::MediaInterface() {
    m_output.setReference(this);
}

manta::MediaInterface::~MediaInterface() {
    /* void */
}
