#include "../include/bsdf.h"

manta::BSDF::BSDF() : ObjectReferenceNode<BSDF>() {
    m_output.setReference(this);
}

manta::BSDF::~BSDF() {
	/* void */
}
