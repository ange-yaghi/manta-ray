#include "media_interface.h"

manta::MediaInterface::MediaInterface() {
	m_output.setMediaInterface(this);
}

manta::MediaInterface::~MediaInterface() {
	/* void */
}

void manta::MediaInterface::registerOutputs() {
	registerOutput(&m_output, "Output");
}
